using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using AVDump2Lib.Hashes;
using System.IO;

namespace AVDump2GUI {
    public partial class GUI : Form {
        private List<FileEntry> files;
        private bool processFiles;
        private List<Hasher> hashers;

        public GUI() {
            InitializeComponent();
        }

        public void AddFiles(List<string> filePaths) {
            FileEntry fileEntry;

            foreach(string filePath in filePaths) {
                for(int i = 0;i < files.Count;i++) if(files[i].FilePath.Path.Equals(filePath)) return;

                fileEntry = new FileEntry();
                fileEntry.FilePath.Path = filePath;

            }
        }


        public eState Processing {
            set { }
        }

        private void ProcessFiles() {
            foreach(FileEntry fileEntry in files) {
                if(fileEntry.Served) continue;

                if(hashers.Count <= 1) {
                    fileEntry.Served = true;
                    ProcessFile(fileEntry); //TODO Create thread
                }
            }
        }

        private void ProcessFile(FileEntry fileEntry) {
            Stream fileStream = System.IO.File.OpenRead(fileEntry.FilePath.Path);
            Hasher hasher = new Hasher(fileStream, 16, 2 * 1024 * 1024);

            if(processFiles) ProcessFiles();
        }

        public enum eState {
            Start,
            Stop,
            Pause,
            Resume
        }
    }
}
