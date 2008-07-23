import wx,os,subprocessw,sys

class mainFrame(wx.Frame):
    __simple = ['p','z','r','m','c','o','9']
    __options = {'avdump':'avdump.exe','user':'','pass':'','ext':'','misc':'','bsize':'2048:16','port':'','tout':'30','retries':'6','exp':'export.txt','log':'log.txt','done':'done.txt'}
    __configfile='config.ini'

    def read_config(self,newconfig=''):
        '''read the config file and return the content'''
        if not os.path.exists(self.__configfile):
            for elem in self.__options:
                newconfig += ('%s=%s\n') %(elem,self.__options[elem])
            file(self.__configfile,"w").write(newconfig)
        for line in file(self.__configfile,"r").readlines():
            if not line.endswith('='):
                temp,val = line.rstrip('\n').split('=',1)
            else:
                val = ''
            for elem in self.__options:
                if temp == elem:
                    self.__options[temp] = val

    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, -1, u'Avdump GUI', wx.DefaultPosition, (350, 140), style=wx.CLOSE_BOX | wx.SYSTEM_MENU | wx.CAPTION | 0 | 0 | wx.MINIMIZE_BOX)
        self.panel = wx.Panel(self, -1)
        self.filepath = wx.TextCtrl(self.panel, -1, '', (95, 25), size=(190, 24))
        self.start = wx.Button(self.panel, -1, 'Start', (90,75), (-1, -1))
        self.options = wx.Button(self.panel, -1, 'Options', (185,75), (-1, -1))
        self.select = wx.Button(self.panel, -1, '...', (295, 25), (25, -1))
        self.scantype = wx.RadioBox(self.panel,-1,pos=(5,5),size=(-1, 59),style=wx.RA_VERTICAL,label='scantype',choices = ['Files','Folder'])
        self.Bind(wx.EVT_BUTTON, self.selectClick, self.select)
        self.Bind(wx.EVT_BUTTON, self.optionsClick, self.options)
        self.Bind(wx.EVT_BUTTON, self.startClick, self.start)
        self.child = OptionsFrame(self, 'App')

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
        self.__options['scanpath'] = path

    def startClick(self, event):
        self.read_config()
        self.__options['param'] = ''
        if self.filepath.GetValue() == '':
            self.child.error('You have to select some files or folder to scan!')
        elif (self.__options['user'] == '' or self.__options['pass'] == ''):
            self.child.error("You didn't enter your username and/or password in the options menu.")
        elif self.__options['avdump'] == '':
            self.child.error('You must specify the path to avdump.exe!')
        elif self.__options['avdump'] == 'avdump.exe':
            self.child.info('You should set the _absolute_ path to avdump.exe.\nOtherwise complications might arise.')
        else:
            if self.__options['misc'].lstrip(',') > 0:
                for elem in self.__options['misc'].split(','):
                    if elem not in ['retries','']:
                        if elem in self.__simple:
                            self.__options['param'] += ' -' + elem
                        elif elem == 'tout':
                            self.__options['param'] += ' -' + elem + ':' + self.__options[elem] + ':' + self.__options['retries']
                        else:
                            self.__options['param'] += ' -' + elem + ':' + self.__options[elem]
            '''debug option to see what avdump actually get's fed'''
            #self.child.info(('%s%s -ac:%s:%s %s') %(self.__options['avdump'], self.__options['param'], self.__options['user'], self.__options['pass'], self.__options['scanpath']))
            arg = (u'%s%s -ac:%s:%s %s') %(self.__options['avdump'], self.__options['param'], self.__options['user'], self.__options['pass'], self.__options['scanpath'])
            subprocessw.Popen(arg)

    def optionsClick(self, event):
        self.child.refresh()
        self.child.Show(True)

class OptionsFrame(wx.Frame):
    __box = {'ext':False,'log':False,'exp':False,'port':False,'done':False,'bsize':False,'tout':False,'c':False,'m':False,'p':False,'r':False,'z':False,'o':True,'9':False}
    __options = {'avdump':'avdump.exe','user':'','pass':'','ext':'','misc':'','bsize':'2048:16','port':'','tout':'30','retries':'6','exp':'export.txt','log':'log.txt','done':'done.txt'}
    __configfile='config.ini'

    def read_config(self):
        '''read the config file and return the content'''
        if not os.path.exists(self.__configfile):
            self.write_config()
        for line in file(self.__configfile,"r").readlines():
            if not line.endswith('='):
                temp,val = line.rstrip('\n').split('=',1)
            else:
                val = ''
            for elem in self.__options:
                if temp == elem:
                    self.__options[temp] = val

    def write_config(self,newconfig=''):
        for elem in self.__options:
            newconfig += ('%s=%s\n') %(elem,self.__options[elem])
            file(self.__configfile,"w").write(newconfig)

    def set_misc(self,misc=''):
        for elem in self.__box:
            if self.__box[elem] is True:
                misc += elem + ','
        self.__options['misc'] = misc

    def get_checkboxes(self,misc):
        if misc is not None:
            misc = misc.rstrip(',').lstrip(',')
            if misc.find(',') > 0:
                for elem in misc.split(','):
                    self.__box[elem] = True
            else:
                self.__box[misc] = True

    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, -1, 'options', wx.DefaultPosition, (301, 691), style=wx.CLOSE_BOX | wx.SYSTEM_MENU | wx.CAPTION | 0 | 0 | wx.MINIMIZE_BOX)
        self.panel = wx.Panel(self, -1)
        self.adoptions = wx.StaticBox(self.panel, -1, "advanced options", (5,170), (275, 290))
        self.groupbox3 = wx.StaticBox(self.panel, -1, "Path to Avdump", (5,0), (275, 55))
        self.groupbox4 = wx.StaticBox(self.panel, -1, "UDP API Username/Password", (5,55), (275, 115))
        self.groupbox5 = wx.StaticBox(self.panel, -1, "logs", (5,460), (275, 170))
        self.filepath = wx.TextCtrl(self.panel, -1, '', (15,20), size=(220, 25))
        self.selectavdump = wx.Button(self.panel, -1, '...', (245,20), (25, 25))
        self.username = wx.TextCtrl(self.panel, -1, '', (15,90), size=(255, 25))
        self.password = wx.TextCtrl(self.panel, -1, '', (15,135), size=(255, 25))
        self.delete = wx.CheckBox(self.panel, -1, 'delete files after parsing', (15,190), (240, 17))
        self.recursive = wx.CheckBox(self.panel, -1, 'do _not_ recurse into subfolders', (15,205), (240, 17))
        self.monitor = wx.CheckBox(self.panel, -1, 'Monitor folder(s)', (15,220), (240, 17))
        self.mp3 = wx.CheckBox(self.panel, -1, 'ostdb mp3 test (experimental)', (15,235), (240, 17))
        self.bsize = wx.CheckBox(self.panel, -1, 'overwrite the default setting for buffersize', (15,250), (240, 17))
        self.ext = wx.CheckBox(self.panel, -1, 'overwrite the default setting for extentions', (15,265), (240, 17))
        self.port = wx.CheckBox(self.panel, -1, 'overwrite the default setting for port', (15,280), (240, 17))
        self.tout = wx.CheckBox(self.panel, -1, 'overwrite the default setting for timeouts', (15,295), (240, 17))
        self.pause = wx.CheckBox(self.panel, -1, 'Pause when done (hold cmd window)', (15,310), (240, 17))
        self.random = wx.CheckBox(self.panel, -1, 'Random file order', (15,325), (240, 17))
        self.wait = wx.CheckBox(self.panel, -1, 'wait for response when sending dumps', (15,340), (240, 17))
        self.extention = wx.TextCtrl(self.panel, -1, '', (15,375), size=(255, 25))
        self.buffersize = wx.TextCtrl(self.panel, -1, '', (15,420), size=(55, 25))
        self.portnumber = wx.TextCtrl(self.panel, -1, '', (81,420), size=(55, 25))
        self.timeout = wx.TextCtrl(self.panel, -1, '', (149,420), size=(55, 25))
        self.retries = wx.TextCtrl(self.panel, -1, '', (215,420), size=(55, 25))
        self.exp = wx.CheckBox(self.panel, -1, '', (15,493), (17, 17))
        self.exppath = wx.TextCtrl(self.panel, -1, '', (35,490), size=(200, 25))
        self.selectexp = wx.Button(self.panel, -1, '...', (245,490), (25, 23))
        self.done = wx.CheckBox(self.panel, -1, '', (15,553), (17, 17))
        self.donepath = wx.TextCtrl(self.panel, -1, '', (35,550), size=(200, 25))
        self.selectdone = wx.Button(self.panel, -1, '...', (245,550), (25, 23))
        self.log = wx.CheckBox(self.panel, -1, '', (15,598), (17, 17))
        self.logpath = wx.TextCtrl(self.panel, -1, '', (35,595), size=(200, 25))
        self.selectlog = wx.Button(self.panel, -1, '...', (245,595), (25, 23))
        self.save = wx.Button(self.panel, -1, 'Save', (65,635), (75, 23))
        self.cancel = wx.Button(self.panel, -1, 'Cancel', (150,635), (75, 23))
        self.Retries = wx.StaticText(self.panel, -1, 'Retries:', (220,402), (44, 17))
        self.Timeout = wx.StaticText(self.panel, -1, 'Timeout:', (152,402), (50, 17))
        self.Buffersize = wx.StaticText(self.panel, -1, 'Buffersize:', (16,402), (60, 17))
        self.Port = wx.StaticText(self.panel, -1, 'Port:', (86,402), (44, 17))
        self.donelog = wx.StaticText(self.panel, -1, 'save processed-file-paths to file and exlude existing from proc', (40,520), (200, 25))
        self.exported2k = wx.StaticText(self.panel, -1, 'export ed2k-links to file', (40,472), (150, 17))
        self.writelog = wx.StaticText(self.panel, -1, 'write output to log', (40,578), (150, 17))
        self.apiuser = wx.StaticText(self.panel, -1, 'Username:', (20,73), (150, 17))
        self.apipass = wx.StaticText(self.panel, -1, 'Password:', (20,118), (150, 17))
        self.extentionlist = wx.StaticText(self.panel, -1, 'comma seperated extension list', (20,358), (170, 17))
        self.Bind(wx.EVT_BUTTON, self.selectavdClick, self.selectavdump)
        self.Bind(wx.EVT_BUTTON, self.selectlogClick, self.selectlog)
        self.Bind(wx.EVT_BUTTON, self.selectexpClick, self.selectexp)
        self.Bind(wx.EVT_BUTTON, self.selectdoneClick, self.selectdone)
        self.Bind(wx.EVT_BUTTON, self.saveClick, self.save)
        self.Bind(wx.EVT_BUTTON, self.cancelClick, self.cancel)
        self.Bind(wx.EVT_CLOSE, self.onclose)
        self.refresh()

    def selectavdClick(self, event):
        dialog = wx.FileDialog ( self, message = 'Pick a directory.')
        if dialog.ShowModal() == wx.ID_OK:
            self.filepath.Clear()
            self.filepath.write(dialog.GetPath().rstrip("\\"))

    def selectlogClick(self, event):
        dialog = wx.FileDialog ( self, message = 'Pick a directory.')
        if dialog.ShowModal() == wx.ID_OK:
            self.logpath.Clear()
            self.logpath.write(dialog.GetPath().rstrip("\\"))

    def selectexpClick(self, event):
        dialog = wx.FileDialog ( self, message = 'Pick a directory.')
        if dialog.ShowModal() == wx.ID_OK:
            self.exppath.Clear()
            self.exppath.write(dialog.GetPath().rstrip("\\"))

    def selectdoneClick(self, event):
        dialog = wx.FileDialog ( self, message = 'Pick a directory.')
        if dialog.ShowModal() == wx.ID_OK:
            self.donepath.Clear()
            self.donepath.write(dialog.GetPath().rstrip("\\"))

    def saveClick(self,event):
        self.update()
        if self.__options['avdump'] == '':
            self.error('You must specify the path to avdump.exe!')
        elif self.__options['avdump'] == 'avdump.exe':
            self.info('You should set the _absolute_ path to avdump.exe.\nOtherwise complications might arise.')
        else:
            self.Show(False)

    def cancelClick(self,event):
        self.refresh()
        self.Show(False)

    def onclose(self,event):
        self.Show(False)

    def refresh(self):
        self.read_config()
        self.get_checkboxes(self.__options['misc'])
        self.filepath.Clear()
        self.filepath.write(self.__options['avdump'].rstrip('"').lstrip('"'))
        self.username.Clear()
        self.username.write(self.__options['user'])
        self.password.Clear()
        self.password.write(self.__options['pass'])
        self.extention.Clear()
        self.extention.write(self.__options['ext'])
        self.buffersize.Clear()
        self.buffersize.write(self.__options['bsize'])
        self.portnumber.Clear()
        self.portnumber.write(self.__options['port'])
        self.timeout.Clear()
        self.timeout.write(self.__options['tout'])
        self.retries.Clear()
        self.retries.write(self.__options['retries'])
        self.exppath.Clear()
        self.exppath.write(self.__options['exp'].rstrip('"').lstrip('"'))
        self.logpath.Clear()
        self.logpath.write(self.__options['log'].rstrip('"').lstrip('"'))
        self.donepath.Clear()
        self.donepath.write(self.__options['done'].rstrip('"').lstrip('"'))
        self.ext.SetValue(self.__box['ext'])
        self.log.SetValue(self.__box['log'])
        self.exp.SetValue(self.__box['exp'])
        self.port.SetValue(self.__box['port'])
        self.done.SetValue(self.__box['done'])
        self.bsize.SetValue(self.__box['bsize'])
        self.tout.SetValue(self.__box['tout'])
        self.recursive.SetValue(self.__box['c'])
        self.monitor.SetValue(self.__box['m'])
        self.mp3.SetValue(self.__box['9'])
        self.pause.SetValue(self.__box['p'])
        self.random.SetValue(self.__box['r'])
        self.delete.SetValue(self.__box['z'])
        self.wait.SetValue(self.__box['o'])

    def update(self):
        self.__options['avdump'] = '"' + self.filepath.GetValue() + '"'
        self.__options['user'] = self.username.GetValue()
        self.__options['pass'] = self.password.GetValue()
        self.__options['ext'] = self.extention.GetValue()
        try: 
            temp,temp2 = self.buffersize.GetValue().split(":")
            temp = int(temp)
            temp2 = int(temp2)
            '''make sure it's mod 8 and at least 256kb'''
            if temp < 256:
                temp = 256
            if not temp % 8:
                temp = temp - (temp%8)
            '''make sure the buffernumber is between 1 and 16'''
            if temp2 > 16 or temp2 <1:
                temp2 = 1
            temp = ("%s:%s") %(temp, temp2)
        except:
            temp = "2048:16"
        self.__options['bsize'] = unicode(temp)
        try:
            int(self.portnumber.GetValue())
            self.__options['port'] = self.portnumber.GetValue()
        except:
            self.__options['port'] = ''
        self.__options['tout'] = self.timeout.GetValue()
        temp = int(self.timeout.GetValue())
        '''make sure it's at least 20'''
        try:
            if int(temp) < 20:
                temp = 20
        except:
            temp = 30
        self.__options['tout'] = unicode(temp)
        try:
            int(self.retries.GetValue())
            self.__options['retries'] = self.retries.GetValue()
        except:
            self.__options['retries'] = 6
        self.__options['exp'] = '"' + self.exppath.GetValue() + '"'
        self.__options['log'] = '"' + self.logpath.GetValue() + '"'
        self.__options['done'] = '"' + self.donepath.GetValue() + '"'
        self.__box['ext'] = self.ext.GetValue()
        self.__box['log'] = self.log.GetValue()
        self.__box['exp'] = self.exp.GetValue()
        self.__box['port'] = self.port.GetValue()
        self.__box['done'] = self.done.GetValue()
        self.__box['bsize'] = self.bsize.GetValue()
        self.__box['tout'] = self.tout.GetValue()
        self.__box['c'] = self.recursive.GetValue()
        self.__box['m'] = self.monitor.GetValue()
        self.__box['9'] = self.mp3.GetValue()
        self.__box['p'] = self.pause.GetValue()
        self.__box['r'] = self.random.GetValue()
        self.__box['z'] = self.delete.GetValue()
        self.__box['o'] = self.wait.GetValue()
        self.set_misc()
        self.write_config()

    def error(self,text):
        message = wx.MessageDialog(self,text,style=wx.ICON_ERROR | wx.OK)
        message.ShowModal()

    def info(self,text):
        message = wx.MessageDialog(self,text,style=wx.ICON_INFORMATION | wx.OK)
        message.ShowModal()
        
class App(wx.App):
    def OnInit(self):
        frame = mainFrame(None, 'App')
        frame.Show(True)
        self.SetTopWindow(frame)
        return True

if __name__ == '__main__':
    app = App(True)
    app.MainLoop()
