import datetime, locale, os, sys
import ConfigParser, esky

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

    _allowed_extentions_globs = ["*." + ext for ext in _allowed_extensions]

    _allowed_extensions_str = "Dumpable Files (%s)" % " ".join(
        _allowed_extentions_globs)

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
        self._export_filename = "../../exports/export_%s.txt" % datetime.datetime.today().strftime("%Y-%m-%d_%H-%M-%S")

        self._ui.datatable.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)

        self.connect(self._ui.files_button, QtCore.SIGNAL("clicked()"), self._slotFile)
        self.connect(self._ui.folder_button, QtCore.SIGNAL("clicked()"), self._slotFolder)
        self.connect(self._ui.start_button, QtCore.SIGNAL("clicked()"), self._run)
        self.connect(self._ui.stop_button, QtCore.SIGNAL("clicked()"), self._stop)
        self.connect(self._ui.datatable, QtCore.SIGNAL("dropped"), self._add_files)
        self.connect(QtGui.QAction(self), QtCore.SIGNAL('triggered()'), QtCore.SLOT('close()'))

        self._enable_elements()
        self._read_config()

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
                    del self._filelist[os.path.join(unicode(self._ui.datatable.item(row,2).text()), unicode(self._ui.datatable.item(row,1).text()))]
                    self._ui.datatable.removeRow(row)
                self._ui.progressBar.setValue(self._calculate_progress())

#################################################
#                                               #
#                UPDATE FUNCTIONS               #
#                                               #
#################################################
    def _update_app(self):
        if getattr(sys, "frozen", False):
            self._updater = Updater()
            self.connect(self._updater, self._updater.SIG_NEWVERSION, self._new_version)
            self.connect(self._updater, self._updater.SIG_RESTART, self._restart_program)
            self.connect(self._updater, self._updater.SIG_PROBLEM, self._raise_updater_error)
            self._updater.check_version()

    def _restart_program(self):
        QtGui.QMessageBox.information(self, "Update done!", "The update was successful. You will need to restart the program now.", QtGui.QMessageBox.Ok)
        #os.execl(sys.executable, sys.executable, * sys.argv)
        sys.exit()

    def _new_version(self):
        self._disable_elements()
        self._ui.stop_button.setDisabled(True)
        QtGui.QMessageBox.information(self, "New Version!", "A new version has been found and will be installed now. You won't be able to use the program until this has finished. Note that it will appear to have locked up for a bit. Just let it finish.", QtGui.QMessageBox.Ok)
        self._updater.start()

    def _raise_updater_error(self, error_message):
        QtGui.QMessageBox.critical(self, "Error!", error_message, QtGui.QMessageBox.Ok)

#################################################
#                                               #
#                 GUI FUNCTIONS                 #
#                                               #
#################################################
    def _resize_columns(self):
        self._ui.datatable.resizeColumnToContents(0)
        self._ui.datatable.resizeColumnToContents(1)
        if self._ui.datatable.columnWidth(0) < 50:
            self._ui.datatable.setColumnWidth(0, 50)
        if self._ui.datatable.columnWidth(1) < 200:
            self._ui.datatable.setColumnWidth(1, 200)

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
            if unicode(self._ui.datatable.item(i,0).text()) == 'done':
                done += 1

        return done*100/self._ui.datatable.rowCount()

    def _refresh_stats(self):
        if self._ui.exp.isChecked():
            self._output_export()
        self._ui.progressBar.setValue(self._calculate_progress())
        self._resize_columns()
        
#################################################
#                                               #
#                 EVENT HANDLER                 #
#                                               #
#################################################
    def resizeEvent(self, event):
        event.accept()
        self._resize_columns()

    def closeEvent(self, event):
        self._stop()
        self._write_config()
        event.accept()

    def _file_hashing(self, path):
        i = self._paths[path]
        self._ui.datatable.setItem(i, 0, QtGui.QTableWidgetItem('hashing'))
        self._resize_columns()

    def _file_sending(self, path):
        i = self._paths[path]
        self._ui.datatable.setItem(i, 0, QtGui.QTableWidgetItem('sending'))
        self._resize_columns()

    def _file_done(self, path):
        i = self._paths[path]
        self._ui.datatable.setItem(i, 0, QtGui.QTableWidgetItem('done'))
        self._refresh_stats()

    def _file_aborted(self, path):
        i = self._paths[path]
        self._ui.datatable.setItem(i, 0, QtGui.QTableWidgetItem('aborted'))
        self._resize_columns()
        self._refresh_stats()

    def _finished(self, exitcode, exitstatus):
        self._refresh_stats()
        self._paths = {}
        self._enable_elements()

    def _raise_error(self, error_message):
        if self._subprocess is not None:
            self._subprocess.stop()
        QtGui.QMessageBox.critical(self, "Error!", error_message, QtGui.QMessageBox.Ok)

    def _stop(self):
        if self._subprocess is not None:
            self._subprocess.stop()
            self._subprocess = None

    def _slotFile(self):
        self._read_done()
        files = QtGui.QFileDialog.getOpenFileNames(self, 'File select', self._last_dir, self._allowed_extensions_str)
        self._add_files(files)

    def _slotFolder(self):
        root_dir = QtGui.QFileDialog.getExistingDirectory(self, "Select Directory", self._last_dir)
        if not root_dir:
            return
        self._read_done()
        diter = QtCore.QDirIterator(root_dir, self._allowed_extentions_globs, QtCore.QDir.Files, QtCore.QDirIterator.Subdirectories)
        for path in iter(diter.next, ""):
            self._add_file(path)

        self._ui.progressBar.setValue(self._calculate_progress())
        self._resize_columns()

    def _add_files(self, files):
        for path in files:
            self._add_file(path)

        self._ui.progressBar.setValue(self._calculate_progress())
        self._resize_columns()

    def _add_file(self, fileloc):
        if os.sep == "\\":
            fileloc = fileloc.replace("/", "\\")

        if self._ui.done.isChecked() and fileloc.normalized(QtCore.QString.NormalizationForm_KC) in self._done_files:
            return

        filepath, filename = os.path.split(unicode(fileloc))

        tmp, ext = os.path.splitext(filename)
        if ext[1:] not in self._allowed_extensions:
            return

        self._last_dir = filepath

        no = self._ui.datatable.rowCount()
        if fileloc not in self._filelist:
            self._filelist[fileloc.normalized(QtCore.QString.NormalizationForm_KC)] =  1
            self._ui.datatable.insertRow(no)

            item = QtGui.QTableWidgetItem('new')
            self._ui.datatable.setItem(no, 0, item)

            item = QtGui.QTableWidgetItem(filename)
            self._ui.datatable.setItem(no, 1, item)

            item = QtGui.QTableWidgetItem(filepath)
            self._ui.datatable.setItem(no, 2, item)

    def _run(self):
        username = unicode(self._ui.username.text())
        apikey   = unicode(self._ui.apikey.text())

        if self._ui.datatable.rowCount() == 0:
            self._raise_error("No files to scan in list.")
            return
        elif not os.path.exists('avdump2cl.exe'):
            self._raise_error("Program avdump2cl.exe not found")
            return
        elif len(username) == 0 or len(apikey) == 0:
            self._raise_error("No username and/or api-key specified!")
            return

        paths    = []
        for i in range(self._ui.datatable.rowCount()):
            if unicode(self._ui.datatable.item(i,0).text()) != 'done':
                path = os.path.join(unicode(self._ui.datatable.item(i,2).text()), unicode(self._ui.datatable.item(i,1).text()))
                paths.append(path)
                self._paths[path] = i

        if len(paths) > 0:
            self._ui.progressBar.setValue(self._calculate_progress())
            self._disable_elements()
            done_file = None
            if self._ui.done.isChecked() is True:
                done_file = "../../done.txt"

            export_file = None
            if self._ui.exp.isChecked() is True:
                if not os.path.exists('../../exports'):
                    os.mkdir('../../exports')
                export_file = self._export_filename

            proc = Avdump(username, apikey, done_file, export_file, paths)
            self.connect(proc, proc.SIG_FILE_HASHING, self._file_hashing)
            self.connect(proc, proc.SIG_FILE_SENDING, self._file_sending)
            self.connect(proc, proc.SIG_FILE_ABORTED, self._file_aborted)
            self.connect(proc, proc.SIG_FILE_DONE, self._file_done)
            self.connect(proc, proc.SIG_FINISHED, self._finished)
            self.connect(proc, proc.SIG_PROBLEM, self._raise_error)
            self._subprocess = proc
            proc.start()

#################################################
#                                               #
#               CONFIG READ/WRITE               #
#                                               #
#################################################
    def _read_config(self):
        if os.path.exists('../../options.ini'):
            config = ConfigParser.ConfigParser()
            config.read('../../options.ini')
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

            try:
                width  = int(self._get_config_bit(config, 'DEFAULT', 'window_width'))
                height = int(self._get_config_bit(config, 'DEFAULT', 'window_height'))
                if height > 1 and width > 1:
                    self.resize(width, height)
            except:
                pass

    def _get_config_bit(self, config, section, option):
        try:
            return config.get(section, option)
        except:
            return ''

    def _write_config(self):
        config = ConfigParser.ConfigParser()
        config.read('../../options.ini')
        config.set("DEFAULT", "username", str(self._ui.username.text()))
        config.set("DEFAULT", "apikey", str(self._ui.apikey.text()))
        config.set('DEFAULT', 'last_dir', self._last_dir)

        if self.isMaximized() is False:
            config.set('DEFAULT', 'window_width', self.width())
            config.set('DEFAULT', 'window_height', self.height())
                
        if self._ui.done.isChecked():
            config.set("DEFAULT", "done", 1)
        else:
            config.set("DEFAULT", "done", 0)
        if self._ui.exp.isChecked():
            config.set("DEFAULT", "exp", 1)
        else:
            config.set("DEFAULT", "exp", 0)
        config.write(file("../../options.ini", "w"))

    def _read_done(self):
        if os.path.exists("../../done.txt"):
            for line in file("../../done.txt"):
                self._done_files.append(QtCore.QString.fromUtf8(line.strip())
                    .normalized(QtCore.QString.NormalizationForm_KC))

class Avdump(QtCore.QProcess):
    """Subprocess using avdump to hash files and communicate with anidb"""

    _procname = "avdump2cl.exe"

    # rar: would be nice to communicate with avdump using a unicode encoding
    #_encoding = locale.getpreferredencoding()
    _encoding = u'utf-8'
    _linesep  = os.linesep.encode(_encoding)

    SIG_FILE_HASHING = QtCore.SIGNAL('avdump_file_hashing')
    SIG_FILE_SENDING = QtCore.SIGNAL('avdump_file_sending')
    SIG_FILE_ABORTED = QtCore.SIGNAL('avdump_file_aborted')
    SIG_FILE_DONE = QtCore.SIGNAL('avdump_file_done')
    SIG_PROBLEM = QtCore.SIGNAL('avdump_problem')

    SIG_FINISHED = QtCore.SIGNAL('finished(int, QProcess::ExitStatus)')

    _SIG_STDOUT_READY = QtCore.SIGNAL('readyReadStandardOutput()')
    _SIG_STDERR_READY = QtCore.SIGNAL('readyReadStandardError()')

    def __init__(self, username, apikey, done_file, export_file, paths):
        QtCore.QProcess.__init__(self)
        self._paths       = paths
        self._status_path = 0 
        self._args        = ["--UseUtf8", "--Quiet", "--Authentication=%s:%s" % (username, apikey)]
        if done_file is not None:
            self._args.append("--DoneLog=" + done_file)
        if export_file is not None:
            self._args.append("--ExportEd2kLinks=" + export_file)
        self._args.extend(paths)
        self._stdout_remainder = self._stderr_remainder = ""
        self._current_folder = self._current_filename = None
        self._done = False

        self.connect(self, self._SIG_STDOUT_READY, self._read_stdout)
        self.connect(self, self._SIG_STDERR_READY, self._read_stderr)
        self.connect(self, self.SIG_FINISHED, self._finished)

    def start(self):
        """Start the avdump child process"""
        QtCore.QProcess.start(self, self._procname, self._args)

    def stop(self):
        """Kill the avdump child process if it's currently running

        Ideally this function would not use kill at all, avdump should listen
        to stdin and terminate when EOF is received from the close call.
        """
        if self.state() != self.NotRunning:
            self.close()
            self.kill()
            self.waitForFinished()

    def _read_stdout(self):
        """Read available avdump output and generate corresponding events"""
        buffer = self._stdout_remainder + unicode(self.readAllStandardOutput())
        assert buffer, "_read_stdout called with nothing to process?"
        lines = buffer.split(self._linesep)
        self._stdout_remainder = lines.pop()
        self._done = False
        for line in lines:
            if __debug__:
                print line
            if line.startswith("Folder: "):
                self._current_folder = line.split(": ", 1)[1]
                self._current_filename = None
            elif line.startswith("Filename: "):
                # rar: ideally get filename from stdout, but encoding woes...
                self._current_filename = line.split(": ", 1)[1]
                self.emit(self.SIG_FILE_HASHING, self._paths[self._status_path])
            elif self._current_filename is not None and line == '':
                self._current_folder = self._current_filename = None
                self.emit(self.SIG_FILE_DONE, self._paths[self._status_path])
                self._done = True
                self._status_path += 1

        if self._done is True:
            if self._status_path < len(self._paths):
                self.emit(self.SIG_FILE_SENDING, self._paths[self._status_path])

    def _read_stderr(self):
        """Read available avdump error output and forward error messages"""
        buffer = unicode(self.readAllStandardError())
        assert buffer, "_read_stderr called with nothing to process?"
        sys.stderr.write(buffer)
        lines = buffer.split(self._linesep)
        sys.stderr.write("\n".join(lines))
        lines[0] = self._stderr_remainder + lines[0]
        self._stderr_remainder = lines.pop()
        for line in lines:
            if "Reason: TimeOut" in line:
                return # avdump will keep trying, not a hard error
            self.emit(self.SIG_PROBLEM, line.decode(self._encoding))

    def _finished(self, exitcode, exitstatus):
        """Generate any events required on process exit"""
        if self._current_filename:
            self.emit(self.SIG_FILE_ABORTED, self._paths[self._status_path])

class Updater(QtCore.QProcess):
    SIG_PROBLEM    = QtCore.SIGNAL('updater_problem')
    SIG_NEWVERSION = QtCore.SIGNAL('updater_new_version')
    SIG_RESTART    = QtCore.SIGNAL('updater_restart')

    def __init__(self):
        QtCore.QProcess.__init__(self)
        self._app = esky.Esky(sys.executable,"http://static.anidb.net/client/avdump2-gui/")
        self._app.cleanup()

    def check_version(self):
        current = self._app.active_version
        try:
            new = self._app.find_update()
            if new is not None:
                if new > current:
                    self.emit(self.SIG_NEWVERSION)
        except Exception, e:
            self.emit(self.SIG_PROBLEM, "ERROR UPDATING APP: " + str(e))        

    def start(self):
        try:
            self._app.auto_update()
            self.emit(self.SIG_RESTART)
        except Exception, e:
            sys.stderr.write("ERROR UPDATING APP: " + str(e))
            self.emit(self.SIG_PROBLEM, "ERROR UPDATING APP: " + str(e))
        self._app.cleanup()

def main():
    app = QtGui.QApplication(sys.argv)
    window=Main()
    window.show()
    window._resize_columns()
    window._update_app()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
