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

'''This file provides APSW-driven database access through Sqlite. There are
a few things to know; read the comment(s) below for details.'''

'''The current state of things as we approach version 3 is not bad at all. We
still need to do some work on stability and flexibility, and there's still a
lot of duplicated code in this file (db.py,) but overall we're making good
progress. The cache works fine, and UDP works as expected. Mylists still need
to be done, but those should be a piece of cake. After mylists are done, some
work should be put in on simplifying and strengthening the code in here, since
this is the part of the program that does the most work with the least clarity.'''

import os.path

import apsw

import config
import udp

version = "Database:\n  APSW version: " + apsw.apswversion() + "\n  SQLite version: " + apsw.sqlitelibversion() + "\n"

conf = config.config()

db = os.path.normpath(os.path.expanduser("~") + "/.oadb/anime.db")

data = {
    "anime":
    ["aid", "eps", "epcount", "spcount", "arating", "avotes", "tmprating", "tmpvotes", "average", "ratings", "year", "type", "aromaji", "akanji", "aenglish", "aother", "ashort", "synonyms", "cats"],
    "episodes":
    ["eid", "aid", "length", "erating", "evotes", "epno", "eenglish", "eromaji", "ekanji"],
    "files":
    ["fid", "aid", "eid", "gid", "state", "size", "ed2k", "filename"],
    "groups":
    ["gid", "grating", "gvotes", "acount", "fcount", "gname", "gshort", "irc", "ircserver", "url"],
    "mylist":
    ["lid", "fid", "eid", "aid", "gid", "date", "state", "viewdate", "storage", "source", "lother"]
}

def regendb():
    '''Regenerates the entire database. Replaces old, shitty functions
    that didn't do their job right.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    for k,v in data.iteritems():
        try:
            cursor.execute("drop table " + k)
        except apsw.SQLError:
            pass
        cursor.execute("vacuum")
        cursor.execute("create table " + k + "(" + ', '.join(v) + ")")

if not os.path.exists(db):
    # There is no os.path.touch(), sadly...
    open(db, "w").close()
    # Gotta recreate the database!
    regendb()

def addaid(animel):
    '''Adds an anime to the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    # Pass one: Turn int4 into int
    for i in range(10):
        animel[i] = int(animel[i])
    # Pass two: turn str into unicode
    for i in range(len(animel)):
        if type(animel[i]) == str:
            animel[i] = animel[i].strip().decode('utf8')
    cursor.execute("insert into anime values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", animel)

def findaid(aid=0, aname=u''):
    '''Find an anime record in the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    '''Lemme explain this line and the one after it, okay? This is for posterity.
    So, the list comprehension returns a list of tuples. We need to check to see
    whether or not we got a row back. The if: after this line tests for that. If
    we got no result, then we wasted an SQL statement; such is life. We search
    again, this time by aname.'''
    result = [dict(zip(data['anime'],row)) for row in cursor.execute("select * from anime where aid=? limit 1", (aid,))]
    # Try to fix a rather nasty bug...
    if aid == 0:
        # Miss; try again.
        result = [dict(zip(data['anime'],row)) for row in cursor.execute("select * from anime where aromaji=? or akanji=? or aenglish=? limit 1", (aname,aname,aname))]
    if result == []:
        # Time to go to the 'Net.
        net = udp.anime(aid, aname)
        if net != None:
            addaid(net)
            if aid != 0:
                result = [dict(zip(data['anime'],row)) for row in cursor.execute("select * from anime where aid=? limit 1", (aid,))]
            else:
                result = [dict(zip(data['anime'],row)) for row in cursor.execute("select * from anime where aromaji=? or akanji=? or aenglish=? limit 1", (aname,aname,aname))]
        else:
            return None
    if (conf.get("english.anime") and result[0]['aenglish'] != '') or result[0]['aromaji'] == '':
        result[0]['aname'] = result[0]['aenglish']
    else:
        result[0]['aname'] = result[0]['aromaji']
    return result[0]

def rmaid(aid=0):
    '''Remove a record from the anime cache.'''
    handle = apsw.Connection(db)
    handle.cursor().execute("delete from anime where aid=?", (aid,))

def allanime(sort):
    '''Returns the anime table.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    if sort == 0:
        result = [dict(zip(data['anime'],row)) for row in cursor.execute("select * from anime order by aid asc")]
    elif sort == 1:
        result = [dict(zip(data['anime'],row)) for row in cursor.execute("select * from anime order by aromaji asc")]
    for dic in result:
        if (conf.get("english.anime") and dic['aenglish'] != '') or dic['aromaji'] == '':
            dic['aname'] = dic['aenglish']
        else:
            dic['aname'] = dic['aromaji']
    return result

def addeid(epl):
    '''Adds an episode to the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    # Pass one: Turn int4 into int
    for i in range(5):
        epl[i] = int(epl[i])
    # Special case for epno (fuckin' epno...)
    try:
        epl[5] = int(epl[5])
    except ValueError:
        pass
    # Pass two: turn str into unicode
    for i in range(len(epl)):
        if type(epl[i]) == str:
            epl[i] = epl[i].strip().decode('utf8')
    cursor.execute("insert into episodes values(?,?,?,?,?,?,?,?,?)", epl)

def findeid(eid=0, aid=(0,0)):
    '''Find an episode record in the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    result = [dict(zip(data["episodes"],row)) for row in cursor.execute("select * from episodes where eid=? limit 1", (eid,))]
    if result == []:
        # Miss; try again.
        result = [dict(zip(data["episodes"],row)) for row in cursor.execute("select * from episodes where aid=? and epno=? limit 1", aid)]
        # Python doesn't dynamically reexamine switches after they start...
        if result == []:
            # 'Net!
            net = udp.episode(eid, aid)
            if net != None:
                addeid(net)
                if eid == 0:
                    result = [dict(zip(data["episodes"],row)) for row in cursor.execute("select * from episodes where aid=? and epno=? limit 1", aid)]
                else:
                    result = [dict(zip(data["episodes"],row)) for row in cursor.execute("select * from episodes where eid=? limit 1", (eid,))]
            else:
                return None
    retval = result[0]
    retval.update(findaid(retval["aid"]))
    if (conf.get("english.eps") and retval['eenglish'] != '') or retval['eromaji'] == '':
        retval['ename'] = retval['eenglish']
    else:
        retval['ename'] = retval['eromaji']
    return retval

def rmeid(eid=0):
    '''Remove a record from the anime cache.'''
    handle = apsw.Connection(db)
    handle.cursor().execute("delete from episodes where eid=?", (eid,))

def alleps(sort):
    '''Returns the aid and aname columns of the episode table.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    if sort == 0:
        result = [dict(zip(data["episodes"],row)) for row in cursor.execute("select * from episodes order by aid asc")]
    elif sort == 1:
        result = [dict(zip(data["episodes"],row)) for row in cursor.execute("select * from episodes order by eromaji asc")]
    for ep in result:
        if (conf.get("english.eps") and ep['eenglish'] != '') or ep['eromaji'] == '':
            ep['ename'] = ep['eenglish']
        else:
            ep['ename'] = ep['eromaji']
        ep.update(findaid(ep["aid"]))
    return result

def addfid(fpl):
    '''Adds an episode to the cache.'''
    print "Entering addfid..."
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    # Pass one: Turn int4 into int
    for i in range(6):
        fpl[i] = int(fpl[i])
    # Pass two: turn str into unicode
    for i in range(len(fpl)):
        if type(fpl[i]) == str:
            fpl[i] = fpl[i].strip().decode('utf8')
    cursor.execute("insert into files values(?,?,?,?,?,?,?,?)", fpl)

def findfid(fid=0, h=('',0)):
    '''Find a file record in the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    result = [dict(zip(data["files"],row)) for row in cursor.execute("select * from files where fid=? limit 1", (fid,))]
    if result == []:
        # Miss; try again.
        result = [dict(zip(data["files"],row)) for row in cursor.execute("select * from files where ed2k=? and size=? limit 1", h)]
        if result == []:
            # Goto 'Net.
            net = udp.file(fid, h)
            if net != None:
                addfid(net)
                if fid != 0:
                    result = [dict(zip(data["files"],row)) for row in cursor.execute("select * from files where fid=? limit 1", (fid,))]
                else:
                    result = [dict(zip(data["files"],row)) for row in cursor.execute("select * from files where ed2k=? and size=? limit 1", h)]
            else:
                return None
    result[0].update(findeid(result[0]["eid"]))
    result[0].update(findgid(result[0]["gid"]))
    return result[0]

def rmfid(fid=0):
    '''Remove a record from the anime cache.'''
    handle = apsw.Connection(db)
    handle.cursor().execute("delete from files where fid=?", (fid,))

def allfiles():
    '''Returns stuff from the files table.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    result = [dict(zip(data["files"],row)) for row in cursor.execute("select * from files order by fid asc")]
    for file in result:
        file.update(findeid(file["eid"]))
        file.update(findgid(file["gid"]))
    return result

def addgid(gpl):
    '''Adds an episode to the cache.'''
    print "Entering addgid..."
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    # Pass one: Turn int4 into int
    for i in range(5):
        gpl[i] = int(gpl[i])
    # Pass two: turn str into unicode
    for i in range(len(gpl)):
        if type(gpl[i]) == str:
            gpl[i] = gpl[i].strip().decode('utf8')
    cursor.execute("insert into groups values(?,?,?,?,?,?,?,?,?,?)", gpl)

def findgid(gid=0):
    '''Find a group record in the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    result = [dict(zip(data["groups"],row)) for row in cursor.execute("select * from groups where gid=? limit 1", (gid,))]
    if result == []:
        # Goto 'Net.
        net = udp.group(gid)
        if net != None:
            addgid(net)
            result = [dict(zip(data["groups"],row)) for row in cursor.execute("select * from groups where gid=? limit 1", (gid,))]
        else:
            return None
    return result[0]

def rmgid(gid=0):
    '''Remove a record from the anime cache.'''
    handle = apsw.Connection(db)
    handle.cursor().execute("delete from groups where gid=?", (gid,))

def addlid(lpl):
    '''Adds an episode to the cache.'''
    print "Entering addlid..."
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    # Pass one: Turn int4 into int
    for i in range(8):
        lpl[i] = int(lpl[i])
    # Pass two: turn str into unicode
    for i in range(len(lpl)):
        if type(lpl[i]) == str:
            lpl[i] = lpl[i].strip().decode('utf8')
    cursor.execute("insert into mylist values(?,?,?,?,?,?,?,?,?,?,?)", lpl)

def findlid(lid=0, fid=0):
    '''Find a file record in the cache.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    result = [dict(zip(data["mylist"],row)) for row in cursor.execute("select * from mylist where lid=? limit 1", (lid,))]
    if result == []:
        # Miss; try again.
        result = [dict(zip(data["mylist"],row)) for row in cursor.execute("select * from mylist where fid=? limit 1", (fid,))]
        if result == []:
            # Goto 'Net.
            net = udp.mylist(lid, h)
            if net != None:
                addlid(net)
                if lid != 0:
                    result = [dict(zip(data["mylist"],row)) for row in cursor.execute("select * from mylist where lid=? limit 1", (lid,))]
                else:
                    result = [dict(zip(data["mylist"],row)) for row in cursor.execute("select * from mylist where fid=? limit 1", (fid,))]
            else:
                return None
    result[0].update(findfid(result[0]["fid"]))
    return result[0]

def rmlid(lid=0):
    '''Remove a record from the anime cache.'''
    handle = apsw.Connection(db)
    handle.cursor().execute("delete from mylist where lid=?", (lid,))

def allmylist():
    '''Returns stuff from the files table.'''
    handle = apsw.Connection(db)
    cursor = handle.cursor()
    result = [dict(zip(data["mylist"],row)) for row in cursor.execute("select * from mylist order by aid, eid asc")]
    for file in result:
        file.update(findfid(file["fid"]))
    return result

def mylistadd(fid, viewed, state):
    '''Avoid a cache miss, if possible!'''
    result = [i for i in apsw.Connection(db).cursor().execute("select state from mylist where fid=? limit 1", (fid,))]
    if result != []:
        if int(result[0][0]) == state:
            return True
    return udp.mylistadd(fid, viewed, state)