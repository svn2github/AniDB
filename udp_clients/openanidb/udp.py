#!/usr/bin/env python
############################################################################
#    Copyright (C) 2007 by Corbin Simpson                                  #
#    MostAwesomeDude@gmail.com                                             #
#                                                                          #
#    This program is free software; you can redistribute it and/or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################

'''Based upon the UDP API definition, located at
http://wiki.anidb.info/w/UDP_API_Definition'''

'''Note: This implementation handles each API call on a by-call basis.
This means more code, but it also means that each different command can handle
all of the return codes itself.'''

import Queue
import socket
import time
import threading
import wx

import db

server = "api.anidb.info"
port = 9000

apiversion = "3"
clientversion = "2"

version = "Network:\n  UDP API version: " + apiversion + "\n  UDP client version: " + clientversion + "\n"

def s2d(string):
    '''Convenience function. Convert a string to a dictionary.'''
    templ = string.split("&")
    dic = dict()
    for pair in templ:
        (key, val) = pair.split("=")
        dic[key] = val
    return dic

def udpsock():
    '''Return a new UDP socket. Be careful with this -- we can't have
    too many sockets floating around!'''
    return socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

class udpthread(threading.Thread):
    '''Thread that runs a UDP request queue. Handles timeouts, errors,
    MTU, and flood control automatically. Not bad for datagrams, huh?'''
    def __init__(self):
        threading.Thread.__init__(self)
        # In...
        self.i = Queue.Queue(5)
        # And out...
        self.o = Queue.Queue(5)
        # Statusbar
        self.status = None
        # Error handler
        self.errno = None
        self.sock = udpsock()
        self.sock.settimeout(20)
    def run(self):
        while True:
            if not self.i.empty():
                if self.status != None:
                    self.status.SetStatusText("Queued: " + str(self.i.qsize()), 1)
                # This thread's in is the system's out.
                outbound = self.i.get(False)
                print "UDP: Sending", outbound
                self.sock.sendto(outbound, (server, port))
                try:
                    inbound = self.sock.recv(1400)
                    print "UDP: Recieved", inbound
                    if inbound[0:3] == "505":
                        s = "Error: Server returned 'Illegal input recieved.'\nThe program's state may not be consistent. Please restart\nthe program, and email the following to the developers:\n\nUDP: Sent " + outbound + "\nUDP: Recieved " + inbound
                        wx.CallAfter(self.errno, s)
                    elif inbound[0:3] == "555":
                        s = "Error: You have been temporarily banned. Common reasons include:\n - You have flooded the server with too many requests\n - You have logged in too many times from this IP without logging out\nBans may last anywhere from 5 minutes to 30 minutes. Please be patient\n and try again later. Thanks!"
                        wx.CallAfter(self.errno, s)
                    self.o.put(inbound.decode("utf8"))
                except socket.timeout:
                    print "UDP: Timeout!"
                    s = "Error: Database timed out. You may be flooding,\nor the server may be down for maintenance.\nPlease wait a second and then try again."
                    wx.CallAfter(self.errno, s)
                    self.o.put(None)
            elif self.status != None:
                self.status.SetStatusText("Idle...", 1)
            time.sleep(2)

# Yes, this is the session string.
session = None
# The thread.
internets = udpthread()

def unamp(str):
    '''Unescapes a string.'''
    return str.replace("\n"," ").strip()

def unicodify(l):
    '''Iterates through a list, and turns each UTF8 string into Unicode.'''
    for i in range(len(l)):
        if type(l[i]) == str:
            l[i] = l[i].decode("utf8")
    return l

def ping():
    '''Ping the database. We do not need a login for this.
    Returns True on success, otherwise False.'''
    internets.i.put("PING")
    data = internets.o.get()
    (code, data) = data.split(" ",1)
    if code == "300":
        # PONG!
        return True
    else:
        # No pong? Doesn't matter why, just that it failed...
        return False

def login(user, passwd):
    '''Start a session. Returns session id on success, None on failure.'''
    global session
    command = "AUTH user=" + user.lower() + "&pass=" + passwd + "&protover=" + apiversion + "&client=openanidb&clientver=" + clientversion
    internets.i.put(command)
    data = internets.o.get()
    (code, data) = data.split(" ", 1)
    if code == "200":
        # Logged in, yay.
        (session, trash) = data.split(" ", 1)
        return True
    elif code == "500":
        # Failed...
        return False
    elif code == "201":
        # New version available!
        (session, trash) = data.split(" ", 1)
        wx.CallAfter(wx.MessageBox("There's a new version of OADB available.\nYou should go upgrade.","New Version Available", wx.OK | wx.ICON_INFORMATION))
        return True
    else:
        # Some other code. Not important right now
        # FIXME: implement all other codes!
        return None

def logout():
    '''End a session.'''
    global session
    if session == None:
        # Why is sanity checking here? Whatever...
        # SQ,SA. It's to keep from issuing a deterministic, wasteful packet.
        return False
    command = "LOGOUT s=" + session
    internets.i.put(command)
    data = internets.o.get()
    (code, data) = data.split(" ", 1)
    if code == "203":
        # Logged out successfully.
        session = None
        return True
    elif code == "403":
        # Well, the session wasn't good, but the end result is the same, I guess.
        session = None
        return True
    else:
        return False

def anime(aid=0, aname=None):
    '''Retrieves data for an anime.'''
    if aid == 0 and aname == None:
        # Failed value
        return False
    elif session == None:
        # If session is empty, then searching the cache is the limit...
        return None
    # Since ID is more accurate than name, it gets done first...
    if aid != 0:
        command = "ANIME aid=" + str(aid) + "&s=" + session
    else:
        command = "ANIME aname=" + aname + "&s=" + session
    ''' I'm gonna rant for a second here.
    So, the MTU (and max. datagram size) for AniDB is 1400 bytes. This is
    absolutely insane. Why? Well, first off, the modern stack implementation
    can send a whole datagram at sizes of 4096 bytes minimum. This means
    that even if fragmentation occurs, the actual hardware can still buffer
    this larger packet without problems. Normally, I wouldn't care, but they
    would rather truncate data than bother trusting IP fragmentation. Sheesh.'''
    internets.i.put(command)
    data = internets.o.get()
    data = unamp(data)
    (code, trash, data) = data.split(" ",2)
    if code == "230":
        datal = data.split("|")
        return datal
    elif code == "330":
        # No such anime.
        return None
    else:
        # FIXME: unsupported return code.
        return None

def episode(eid=0, aid=()):
    '''Gets an episode. Supports the caching mechanism.'''
    if eid == 0 and aid == ():
        # Failed value
        return False
    elif session == None:
        return None
    # Since ID is more accurate than name, it gets done first...
    if eid != 0:
        command = "EPISODE eid=" + str(eid) + "&s=" + session
    else:
        # Sanest way to pass epno
        command = "EPISODE aid=" + str(aid[0]) + "&epno=" + str(aid[1]).lstrip("0") + "&s=" + session
    internets.i.put(command)
    data = internets.o.get()
    data = unamp(data)
    (code, trash, data) = data.split(" ",2)
    if code == "240":
        datal = data.split("|")
        return datal
    elif code == "340":
        # No such episode.
        return None
    else:
        # FIXME: unsupported return code.
        return None

def file(fid=0, file=()):
    '''Issues a FILE command.'''
    if fid == 0 and file == ():
        # WTF?
        return False
    elif session == None:
        # If session is empty, then searching the cache is the limit...
        return None
    if fid != 0:
        command = "FILE fid=" + str(fid) + "&s=" + session
    else:
        command = "FILE size=" + str(file[1]) + "&ed2k=" + file[0].lower() + "&s=" + session
    internets.i.put(command)
    data = internets.o.get()
    data = unamp(data)
    (code, trash, data) = data.split(" ",2)
    if code == "220":
        datal = data.split("|")
        return datal
    elif code == "320":
        # No such file.
        return None
    else:
        # FIXME: unsupported return code.
        return None

def group(gid=0):
    '''Issues a FILE command.'''
    if gid == 0:
        return False
    elif session == None:
        return None
    elif gid != 0:
        command = "GROUP gid=" + str(gid) + "&s=" + session
    internets.i.put(command)
    data = internets.o.get()
    data = unamp(data)
    (code, trash, data) = data.split(" ",2)
    if code == "250":
        datal = data.split("|")
        return datal
    elif code == "350":
        # No such file.
        return None
    else:
        # FIXME: unsupported return code.
        return None

def mylist(lid=0, fid=0):
    '''Issues a MYLIST command.'''
    if lid == 0 and fid == 0:
        # WTF?
        return False
    elif session == None:
        # If session is empty, then searching the cache is the limit...
        return None
    if fid != 0:
        command = "FILE fid=" + str(fid) + "&s=" + session
    else:
        command = "FILE lid=" + str(lid) + "&s=" + session
    internets.i.put(command)
    data = internets.o.get()
    data = unamp(data)
    (code, trash, data) = data.split(" ",2)
    if code == "221":
        datal = data.split("|")
        return datal
    elif code == "321":
        # No such file.
        return None
    else:
        # FIXME: unsupported return code.
        return None

def mylistadd(fid, viewed, state):
    '''Issues a MYLISTADD.'''
    # Do we need to issue a MYLIST anyway? Currently, no.
    if session == None:
        return False
    command = "MYLISTADD fid=" + str(fid) + "&viewed=" + str(viewed) + "&state=" + str(state) + "&s=" + session
    internets.i.put(command)
    code = unamp(internets.o.get())[0:3]
    if code == "210":
        # Construct a fake array of data to assemble and insert into the database.
        x = db.findfid(fid)
        # I'm lazy, so one line. Look at this...
        # ["lid", "fid", "eid", "aid", "gid", "date", "state", "viewdate", "storage", "source", "lother"]
        temp = [code.split(' ')[-1], x["fid"], x["eid"], x["aid"], x["gid"], 0, int(state), 0, '', '', '']
        db.addlid(temp)
        return True
    elif code == "310":
        # Try again, with edit=1.
        lid = code.split(' ')[-1]
        command = "MYLISTADD lid=" + str(lid) + "&viewed=" + str(viewed) + "&state=" + str(state) + "&edit=1&s=" + session
        internets.i.put(command)
        code = unamp(internets.o.get())[0:3]
        if code == "311":
            # O RLY? Well, let's see this so-called "entry," and then do some progressive caching.
            # Construct a fake array of data to assemble and insert into the database.
            x = db.findfid(fid)
            # I'm lazy, so one line. Look at this...
            # ["lid", "fid", "eid", "aid", "gid", "date", "state", "viewdate", "storage", "source", "lother"]
            temp = [code.split(' ')[-1], x["fid"], x["eid"], x["aid"], x["gid"], 0, int(state), 0, '', '', '']
            db.addlid(temp)
            return True
        else:
            return False
    else:
        # FIXME: unsupported return code.
        return None

def encoding():
    '''Sets session encoding. There is no reason to change it away from
    UTF8 Unicode, so don't touch!'''
    command = "ENCODING name=UTF8&s=" + session
    internets.i.put(command)
    code = internets.o.get()[0:3]
    if code == "219":
        return True
    else:
        return False