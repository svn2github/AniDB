import wx,os
from avdump import *

class mainFrame(wx.Frame, AvdumpHandler):
    _configfile='config.ini'

    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, -1, u'Avdump GUI')
        self.panel = wx.Panel(self)
        self.filepath = wx.TextCtrl(self.panel, -1, '')
        self.start = wx.Button(self.panel, -1, 'Start')
        self.select = wx.Button(self.panel, -1, '...')
        self.scantype = wx.RadioBox(self.panel, -1, style=wx.RA_VERTICAL, label='scantype', choices=['Files','Folder'])
        self.username = wx.TextCtrl(self.panel, -1, '')
        self.password = wx.TextCtrl(self.panel, -1, '')
        self.apiuser = wx.StaticText(self.panel, -1, 'Username:')
        self.apipass = wx.StaticText(self.panel, -1, 'UDP API-Key:')
        self.apilink = wx.HyperlinkCtrl(self.panel, -1, "If you don\'t have an UDP API Key set yet, click here to set it", "http://anidb.net/perl-bin/animedb.pl?show=profile#tab4")
        self.ed2kbox = wx.TextCtrl(self.panel, -1, "", style = wx.TE_MULTILINE | wx.TE_READONLY)
        self.Bind(wx.EVT_BUTTON, self.selectClick, self.select)
        self.Bind(wx.EVT_BUTTON, self.startClick, self.start)
        self.read_config()

        mainbox = wx.BoxSizer(wx.VERTICAL)
        addbox = wx.BoxSizer(wx.HORIZONTAL)

        addbox.Add(self.scantype, 0, wx.ALL, 5)
        addbox.Add(self.filepath, 1, wx.ALL | wx.ALIGN_CENTER_VERTICAL, 5)
        addbox.Add(self.select, 0, wx.ALL | wx.ALIGN_CENTER_VERTICAL, 5)

        logingrid = wx.GridSizer(2, 2, 5, 5)
        logingrid.Add(self.apiuser, 0, wx.ALL, 5)
        logingrid.Add(self.apipass, 0, wx.ALL, 5)
        logingrid.Add(self.username, 0, wx.ALL, 5)
        logingrid.Add(self.password, 0, wx.ALL, 5)

        mainbox.Add(addbox, 0, wx.ALL | wx.EXPAND, 5)
        mainbox.Add(logingrid, 0, wx.ALL | wx.ALIGN_CENTER, 5)
        mainbox.Add(self.start, 0, wx.ALL | wx.ALIGN_CENTER, 5)
        mainbox.Add(self.ed2kbox, 1, wx.ALL | wx.EXPAND, 5)
        mainbox.Add(self.apilink, 0, wx.ALL | wx.ALIGN_RIGHT, 5)

        self.panel.SetSizer(mainbox)
        mainbox.SetSizeHints(self)

        self.avdump = AvdumpThread(self)
        self.avdump.start()

    def read_config(self):
        '''read the config file and return the content'''
        if os.path.exists(self._configfile):
            for line in file(self._configfile,"r"):
                try:
                    temp,val = line.rstrip('\n').split('=',1)
                    if temp == "user":
                        self.username.write(val)
                    elif temp == "pass":
                        self.password.write(val)
                except:
                    pass

    def write_config(self):
        newconfig = ''
        newconfig += ('%s=%s\n') %("user",self.username.GetValue())
        newconfig += ('%s=%s\n') %("pass",self.password.GetValue())
        file(self._configfile,"w").write(newconfig)

    def selectClick(self, event,path=''):
        if self.scantype.GetStringSelection() == 'Files':
            dialog = wx.FileDialog ( self, message = 'Pick a directory.', style= wx.FD_MULTIPLE)
            if dialog.ShowModal() == wx.ID_OK:
                temp = dialog.GetPaths()
                if len(temp)>1:
                    for elem in temp:
                        path += '"' + elem + '" '
                else:
                    path = '"' + temp[0].rstrip("\\") + '"'
        else:
            dialog = wx.DirDialog ( self, message = 'Pick a directory.' )
            if dialog.ShowModal() == wx.ID_OK:
                path = '"' + dialog.GetPath().rstrip("\\") + '"'
        self.filepath.Clear()
        dialog.Destroy()
        self.filepath.write(path)

    def startClick(self, event):
        avdump = self.avdump
        avdump.avdumppath = os.path.join(os.getcwd(), "avdump.exe")
        avdump.exportpath = os.path.join(os.getcwd(), "ed2klinks.txt")
        avdump.username = self.username.GetValue()
        avdump.apikey = self.password.GetValue()
        self.write_config()
        if self.filepath.GetValue() == '':
            self.error('You have to select some files or folder to scan!')
        elif (self.username.GetValue() == '' or self.password.GetValue() == ''):
            self.error("You didn't enter your username and/or password in the options menu.")
        else:
            avdump.queue(self.filepath.GetValue())

    def error(self,text):
        message = wx.MessageDialog(self,text,style=wx.ICON_ERROR | wx.OK)
        message.ShowModal()

    def avdump_idle(self):
        pass

    def avdump_ed2k(self, ed2k):
        def doit():
            self.ed2kbox.AppendText(ed2k)
        wx.CallAfter(doit)
        pass
 
class App(wx.App):
    def OnInit(self):
        frame = mainFrame(None, 'App')
        frame.Show(True)
        self.SetTopWindow(frame)
        return True

if __name__ == '__main__':
    app = App(True)
    app.MainLoop()
