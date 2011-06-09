import os, sys, datetime, unicodedata
import ConfigParser

from PyQt4 import QtCore, QtGui

# Import the compiled UI module
from gui import Ui_MainWindow


class Main(QtGui.QMainWindow):
    """Main window control for avidump GUI frontend"""

    _allowed_extensions = frozenset(('7z', 'aac', 'ac3', 'ace', 'asf', 'ass',
        'avi', 'dts', 'dtshd', 'flac', 'flv', 'idx', 'js', 'lrc', 'm2ts',
        'm4a', 'mk3d', 'mka', 'mks', 'mkv', 'mov', 'mp3', 'mp4', 'mpeg',
        'mpg', 'ogg', 'ogm', 'ogv', 'pjs', 'qt', 'ra', 'rar', 'rm', 'rmvb',
        'rt', 'smi', 'smil', 'srt', 'ssa', 'sub', 'swf', 'thd', 'tmp', 'ts',
        'tts', 'txt', 'wav', 'webm', 'wma', 'wmv', 'xss', 'zip'))

    _allowed_extensions_str = "Dumpable Files (%s)" % " ".join(
        "*." + ext for ext in sorted(_allowed_extensions))

    def __init__(self):
        QtGui.QMainWindow.__init__(self)
        self._ui = Ui_MainWindow()
        self._ui.setupUi(self)
        self._subprocess = None
        self._filelist = {}
        self._paths = {}
        self._done_files = []
        self._worker = None
        self._last_dir = os.getcwd()
        self._export_filename = "exports/export_%s.txt" % datetime.datetime.today().strftime("%Y-%m-%d_%H-%M-%S")

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
                    del self._filelist[os.path.join(unicode(self._ui.datatable.item(row,0).text()), unicode(self._ui.datatable.item(row,1).text()))]
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
        self._enable_elements()
        if self._ui.exp.isChecked():
            self._output_export()

    def _raise_error(self, path):
        i = self._paths[path]
        item = QtGui.QTableWidgetItem('error')
        self._ui.datatable.setItem(i, 2, item)
        QtGui.QMessageBox.information(self, "Error!", "Either the client is outdated or your username/password combination is wrong.", QtGui.QMessageBox.Ok)
        self._enable_elements()

    def _stop(self):
        if self._subprocess is not None:
            self._subprocess.stop()
            self._subprocess = None
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

        if os.sep == "\\":
            fileloc = fileloc.replace("/", "\\")

        filepath, filename = os.path.split(fileloc)

        tmp, ext = os.path.splitext(filename)
        if ext[1:] not in self._allowed_extensions:
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
            done_file = None
            if self._ui.done.isChecked() is True:
                done_file = "done.txt"

            export_file = None
            if self._ui.exp.isChecked() is True:
                if not os.path.exists('exports'):
                    os.mkdir('exports')
                export_file = self._export_filename

            self._subprocess = avdump(username, apikey, done_file, export_file, paths)
            self.connect(self._subprocess, QtCore.SIGNAL("done"), self._done)
            self.connect(self._subprocess, QtCore.SIGNAL("finished"), self._finished)
            self.connect(self._subprocess, QtCore.SIGNAL("error"), self._raise_error)
            self._subprocess.start()

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


class avdump(QtCore.QProcess):
    """Subprocess using avdump to hash files and communicate with anidb"""

    _procname = "avdump2cl.exe"

    def __init__(self, username, apikey, done_file, export_file, paths):
        QtCore.QProcess.__init__(self)
        self._paths       = paths
        self._status_path = 0 
        self._args        = ["-w", "-ac:%s:%s" % (username, apikey)]
        if done_file is not None:
            self._args.append("-done:" + done_file)
        if export_file is not None:
            self._args.append("-exp:" + export_file)
        self._args.extend(paths)
        self.stdout  = ''

        self.connect(self, QtCore.SIGNAL('readyReadStandardError()'), self._readStderr)
        self.connect(self, QtCore.SIGNAL('readyReadStandardOutput()'), self._readStdout)
        self.connect(self, QtCore.SIGNAL('finished(int)'), self._finished)

    def _readStdout(self):
        out = self.readAllStandardOutput()
        if out.startsWith("Done"): # ugly hack
            self.emit(QtCore.SIGNAL('done'), self._paths[self._status_path])
            self._status_path += 1
        self.stdout += out

    def _readStderr(self):
        print "error", self.readAllStandardError()

    def _finished(self, exitcode):
        # TODO: make error state depend on exitcode not sniffing the output
        if self._error_happened(self.stdout) is True:
            self.emit(QtCore.SIGNAL('error'), self._paths[self._status_path])
            return
        if self.exitStatus() == self.NormalExit:
            self.emit(QtCore.SIGNAL('finished'))

    def _error_happened(self, stdout):
        if "Either the client is outdated or your username/password combination is wrong" in stdout:
            return True
        else:
            return False

    def stop(self):
        if self.state() != self.NotRunning:
            self.kill()
            self.waitForFinished()

    def start(self):
        QtCore.QProcess.start(self, self._procname, self._args)


def main():
    app = QtGui.QApplication(sys.argv)
    window=Main()
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
