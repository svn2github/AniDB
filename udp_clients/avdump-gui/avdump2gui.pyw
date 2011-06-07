import sys, os, platform, ConfigParser, string, unicodedata

from time import time

# Import Qt modules
from PyQt4 import QtCore, QtGui

# Import the compiled UI module
from gui import Ui_MainWindow

# Create a class for our main window
class Main(QtGui.QMainWindow):
    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self._ui = Ui_MainWindow()
        self._ui.setupUi(self)
        self._filelist = {}
        self._paths = {}
        self._done_files = []
        self._worker = None
        self._last_dir = os.getcwd()
        self._export_filename = 'exports/export_' + str(int(time())) + '.txt'
        self._allowed_extensions = ('avi', 'mpg', 'mpeg', 'ts', 'm2ts', 'rm', 'rmvb', 'asf', 'wmv', 'mov', 'qt', 'ogm',
                                   'mp4', 'mkv', 'swf', 'flv', 'ogv', 'webm', 'mk3d', 'srt', 'sub', 'ssa', 'smi', 'idx', 'ass',
                                   'txt', 'mks', 'lrc', 'rt', 'tmp', 'js', 'pjs', 'tts', 'xss', 'mp3', 'aac', 'ac3', 'dts', 'wav',
                                   'flac', 'wma', 'mka', 'ra', 'ogg', 'm4a', 'dtshd', 'thd', 'rar', 'zip', 'ace', '7z', 'smil')

        tmp = []
        for ext in self._allowed_extensions:
            tmp.append('*.' + ext)

        self._allowed_extensions_str = "Dumpable Files (" + " ".join(tmp) + ")"

        self._ui.datatable.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
        self._ui.datatable.horizontalHeader().setResizeMode(QtGui.QHeaderView.Stretch)

        self.connect(self._ui.files_button, QtCore.SIGNAL("clicked()"), self._slotFile)
        self.connect(self._ui.folder_button, QtCore.SIGNAL("clicked()"), self._slotFolder)
        self.connect(self._ui.start_button, QtCore.SIGNAL("clicked()"), self._run)
        self.connect(self._ui.stop_button, QtCore.SIGNAL("clicked()"), self._stop)
        self.connect(QtGui.QAction(self), QtCore.SIGNAL('triggered()'), QtCore.SLOT('close()'))

        self._enable_elements()
        self._read_config()
        self._read_done()

    def keyPressEvent(self, event):
        if event.key() == QtCore.Qt.Key_Delete:
            selected = self._ui.datatable.selectedIndexes()
            if len(selected) > 0:
                rows = {}
                for item in selected:
                    rows[item.row()] = 1
                rows = rows.keys()
                rows.sort()
                rows.reverse()
                for row in rows:
                    self._ui.datatable.removeRow(row)
                self._ui.progressBar.setValue(self._calculate_progress())

#################################################
#                                               #
#                 GUI FUNCTIONS                 #
#                                               #
#################################################

    def _disable_elements(self):
        self._ui.username.setDisabled(True)
        self._ui.apikey.setDisabled(True)
        self._ui.files_button.setDisabled(True)
        self._ui.folder_button.setDisabled(True)
        self._ui.done.setDisabled(True)
        self._ui.exp.setDisabled(True)
        self._ui.start_button.setDisabled(True)
        self._ui.stop_button.setEnabled(True)

    def _enable_elements(self):
        self._ui.username.setEnabled(True)
        self._ui.apikey.setEnabled(True)
        self._ui.files_button.setEnabled(True)
        self._ui.folder_button.setEnabled(True)
        self._ui.done.setEnabled(True)
        self._ui.exp.setEnabled(True)
        self._ui.start_button.setEnabled(True)
        self._ui.stop_button.setDisabled(True)

    def _output_export(self):
        if os.path.exists(self._export_filename):
            self._ui.ed2k_export.setText("".join(file(self._export_filename).readlines()).decode('utf8'))

    def _calculate_progress(self):
        if self._ui.datatable.rowCount() == 0:
            return 0

        done = 0
        for i in range(self._ui.datatable.rowCount()):
            if unicode(self._ui.datatable.item(i,2).text()) == 'done':
                done += 1

        return done*100/self._ui.datatable.rowCount()             

#################################################
#                                               #
#                 EVENT HANDLER                 #
#                                               #
#################################################
    def closeEvent(self,event):
        self._stop()
        self._write_config()
        event.accept()

    def _done(self, path):
        i = self._paths[path]
        item = QtGui.QTableWidgetItem('done')
        self._ui.datatable.setItem(i, 2, item)
        if self._ui.exp.isChecked():
            self._output_export()
        self._ui.progressBar.setValue(self._calculate_progress())

    def _finished(self):
        self._paths = {}
        self._ui.progressBar.setValue(self._calculate_progress())
        self._worker = None
        self._enable_elements()
        if self._ui.exp.isChecked():
            self._output_export()

    def _raise_error(self, path):
        i = self._paths[path]
        item = QtGui.QTableWidgetItem('error')
        self._ui.datatable.setItem(i, 2, item)
        QtGui.QMessageBox.information(self, "Error!", "Either the client is outdated or your username/password combination is wrong.", QtGui.QMessageBox.Ok)

    def _stop(self):
        if self._worker is not None:
            self._worker.stop()
            self._output_export()
            self._enable_elements()

    def _slotFile(self):
        files = QtGui.QFileDialog.getOpenFileNames(self, 'File select', self._last_dir, self._allowed_extensions_str)
        for path in files:
            self._add_file(unicode(path))

    def _slotFolder(self):
        path = unicode(QtGui.QFileDialog.getExistingDirectory(self, "Select Directory", self._last_dir))
        if path != '':
            for root, dir, files in os.walk(path):
                for filename in files:
                    self._add_file(os.path.join(root, filename))

    def _add_file(self, fileloc):
        if len(fileloc) == 0:
            return
        elif self._ui.done.isChecked() is True and unicodedata.normalize('NFKC', fileloc) in self._done_files:
            return

        no = self._ui.datatable.rowCount()

        if platform.system().lower() == 'windows':
            fileloc = fileloc.replace("/", "\\")
            filepath, filename = fileloc.rsplit("\\", 1)
        else:
            filepath, filename = fileloc.rsplit("/", 1)

        tmp, ext = filename.rsplit('.', 1)
        if ext not in self._allowed_extensions:
            return

        self._last_dir = filepath

        if fileloc not in self._filelist.keys():
            self._filelist[fileloc] =  1
            self._ui.datatable.insertRow(no)

            item = QtGui.QTableWidgetItem(filepath)
            self._ui.datatable.setItem(no, 0, item)

            item = QtGui.QTableWidgetItem(filename)
            self._ui.datatable.setItem(no, 1, item)

            item = QtGui.QTableWidgetItem('new')
            self._ui.datatable.setItem(no, 2, item)
            self._ui.progressBar.setValue(self._calculate_progress())

    def _run(self):
        username = unicode(self._ui.username.text())
        apikey   = unicode(self._ui.apikey.text())

        if self._ui.datatable.rowCount() == 0:
            QtGui.QMessageBox.information(self, "Error!", "No files to scan in list.", QtGui.QMessageBox.Ok)
            return
        elif not os.path.exists('avdump2cl.exe'):
            QtGui.QMessageBox.information(self, "Error!", "Avdump2cl.exe not found.", QtGui.QMessageBox.Ok)
            return
        elif len(username) == 0 or len(apikey) == 0:
            QtGui.QMessageBox.information(self, "Error!", "No username and/or api-key specified!", QtGui.QMessageBox.Ok)
            return

        paths    = []
        for i in range(self._ui.datatable.rowCount()):
            if unicode(self._ui.datatable.item(i,2).text()) != 'done':
                path = os.path.join(unicode(self._ui.datatable.item(i,0).text()), unicode(self._ui.datatable.item(i,1).text()))
                paths.append(path)
                self._paths[path] = i

        if len(paths) > 0:
            self._ui.progressBar.setValue(self._calculate_progress())
            self._disable_elements()
            done     = ''
            if self._ui.done.isChecked() is True:
                done = '-done:done.txt'

            exp      = ''
            if self._ui.exp.isChecked() is True:
                if not os.path.exists('exports'):
                    os.mkdir('exports')
                exp  = '-exp:' + self._export_filename
                
            self._worker = avdump_worker(username, apikey, done, exp, paths)
            self.connect(self._worker, QtCore.SIGNAL("done"), self._done)
            self.connect(self._worker, QtCore.SIGNAL("finished"), self._finished)
            self.connect(self._worker, QtCore.SIGNAL("error"), self._raise_error)
            self._worker.start()           

#################################################
#                                               #
#               CONFIG READ/WRITE               #
#                                               #
#################################################
    def _read_config(self):
        if os.path.exists('options.ini'):
            config = ConfigParser.ConfigParser()
            config.read('options.ini')
            self._ui.username.setText(self._get_config_bit(config, 'DEFAULT', 'username'))
            self._ui.apikey.setText(self._get_config_bit(config, 'DEFAULT', 'apikey'))
            if self._get_config_bit(config, 'DEFAULT', 'done') == '1':
                self._ui.done.setChecked(True)
            else:
                self._ui.done.setChecked(False)

            if self._get_config_bit(config, 'DEFAULT', 'exp') == '1':
                self._ui.exp.setChecked(True)
            else:
                self._ui.exp.setChecked(False)

            self._last_dir = self._get_config_bit(config, 'DEFAULT', 'last_dir')

    def _get_config_bit(self, config, section, option):
        try:
            return config.get(section, option)
        except:
            return ''

    def _write_config(self):
        config = ConfigParser.ConfigParser()
        config.set("DEFAULT", "username", str(self._ui.username.text()))
        config.set("DEFAULT", "apikey", str(self._ui.apikey.text()))
        config.set('DEFAULT', 'last_dir', self._last_dir)
        if self._ui.done.isChecked():
            config.set("DEFAULT", "done", 1)
        else:
            config.set("DEFAULT", "done", 0)
        if self._ui.exp.isChecked():
            config.set("DEFAULT", "exp", 1)
        else:
            config.set("DEFAULT", "exp", 0)
        config.write(file("options.ini", "w"))

    def _read_done(self):
        if os.path.exists("done.txt"):
            for line in file("done.txt"):
                self._done_files.append(unicodedata.normalize('NFKC', line.decode("utf8").strip()))

class avdump_worker(QtCore.QThread):
    def __init__(self, username, apikey, done, exp, paths):
        QtCore.QThread.__init__(self, parent=None)
        self._paths       = paths
        self._was_stopped = False
        self._isrunning   = False
        self._args        = (u'avdump2cl.exe -w -ac:%s:%s %s %s') %(username, apikey, done, exp)

    def _error_happened(self, stdout):
        if "Either the client is outdated or your username/password combination is wrong" in stdout:
            self._isrunning = False
            return True
        else:
            return False

    def stop(self):
        self._was_stopped = True
        if self._isrunning is True:
            self._avdump.kill()

    def run(self):
        self._isrunning = True
        self._avdump = avdump(self._args)
        for path in self._paths:
            if self._was_stopped is True or self._isrunning is False:
                break

            stdout = self._avdump.dump(path)
            if self._error_happened(stdout) is True:
                self.emit(QtCore.SIGNAL('error'), path)
                self._isrunning = False
                break
            elif self._was_stopped is True:
                break
            else:
                self.emit(QtCore.SIGNAL('done'), path)

        self._isrunning = False

        if self._was_stopped is False:
            self.emit(QtCore.SIGNAL('finished'))

class avdump():
    def __init__(self, args):
        self._avdump = QtCore.QProcess()
        self._args   = args

    def dump(self, path):
        self._isrunning = True
        self._avdump.start((u'%s "%s"') %(self._args, path))
        self._avdump.waitForFinished()
        stdout = self._avdump.readAll()
        self._avdump.close()
        self._isrunning = False
        return stdout

    def kill(self):
        if self._isrunning:
            self._avdump.kill()
        
def main():
    app = QtGui.QApplication(sys.argv)
    window=Main()
    window.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
