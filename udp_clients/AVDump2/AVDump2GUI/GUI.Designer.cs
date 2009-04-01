namespace AVDump2GUI {
    partial class GUI {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.tbctrl_Main = new System.Windows.Forms.TabControl();
            this.tp_Files = new System.Windows.Forms.TabPage();
            this.listView1 = new System.Windows.Forms.ListView();
            this.panel1 = new System.Windows.Forms.Panel();
            this.grp_FileInfo = new System.Windows.Forms.GroupBox();
            this.lnklbl_GotoAniDB = new System.Windows.Forms.LinkLabel();
            this.lbl_Size = new System.Windows.Forms.Label();
            this.btn_CopyHashes = new System.Windows.Forms.Button();
            this.chcklst_CopyHashes = new System.Windows.Forms.CheckedListBox();
            this.lbl_CopyAll = new System.Windows.Forms.Label();
            this.flp_Hashes = new System.Windows.Forms.FlowLayoutPanel();
            this.pnl_Ed2k = new System.Windows.Forms.Panel();
            this.lbl_Ed2kHash = new System.Windows.Forms.Label();
            this.lbl_Ed2k = new System.Windows.Forms.Label();
            this.lnklbl_CopyEd2kHash = new System.Windows.Forms.LinkLabel();
            this.pnl_Sha1 = new System.Windows.Forms.Panel();
            this.lbl_Sha1Hash = new System.Windows.Forms.Label();
            this.lbl_Sha1 = new System.Windows.Forms.Label();
            this.lnklbl_CopySha1Hash = new System.Windows.Forms.LinkLabel();
            this.pnl_Md5 = new System.Windows.Forms.Panel();
            this.lbl_Md5Hash = new System.Windows.Forms.Label();
            this.lbl_Md5 = new System.Windows.Forms.Label();
            this.lnklbl_CopyMd5Hash = new System.Windows.Forms.LinkLabel();
            this.pnl_Crc = new System.Windows.Forms.Panel();
            this.lbl_CrcHash = new System.Windows.Forms.Label();
            this.lbl_Crc = new System.Windows.Forms.Label();
            this.lnklbl_CopyCrcHash = new System.Windows.Forms.LinkLabel();
            this.pnl_Tth = new System.Windows.Forms.Panel();
            this.lbl_TthHash = new System.Windows.Forms.Label();
            this.lbl_Tth = new System.Windows.Forms.Label();
            this.lnklbl_CopyTthHash = new System.Windows.Forms.LinkLabel();
            this.pnl_Aich = new System.Windows.Forms.Panel();
            this.lbl_AichHash = new System.Windows.Forms.Label();
            this.Aich = new System.Windows.Forms.Label();
            this.lnklbl_CopyAichHash = new System.Windows.Forms.LinkLabel();
            this.lnklbl_CopyAll = new System.Windows.Forms.LinkLabel();
            this.lbl_Margin = new System.Windows.Forms.Label();
            this.Settings = new System.Windows.Forms.GroupBox();
            this.chcklst_Hashes = new System.Windows.Forms.CheckedListBox();
            this.tp_Settings = new System.Windows.Forms.TabPage();
            this.grp_AVDump2GUISettings = new System.Windows.Forms.GroupBox();
            this.lbl_UseColumns = new System.Windows.Forms.Label();
            this.chcklst_Columns = new System.Windows.Forms.CheckedListBox();
            this.grp_AVDump2Settings = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lbl_BlockSettings = new System.Windows.Forms.Label();
            this.lbl_ACreqSettings = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lbl_UserPassword = new System.Windows.Forms.Label();
            this.lbl_Username = new System.Windows.Forms.Label();
            this.lbl_APIHost = new System.Windows.Forms.Label();
            this.lbl_UserPort = new System.Windows.Forms.Label();
            this.chck_ACreq = new System.Windows.Forms.CheckBox();
            this.nup_BlockCount = new System.Windows.Forms.NumericUpDown();
            this.nup_CoreCount = new System.Windows.Forms.NumericUpDown();
            this.chck_IntelliProc = new System.Windows.Forms.CheckBox();
            this.nup_BlockSize = new System.Windows.Forms.NumericUpDown();
            this.nup_Timeout = new System.Windows.Forms.NumericUpDown();
            this.nup_Retries = new System.Windows.Forms.NumericUpDown();
            this.txt_APIHost = new System.Windows.Forms.TextBox();
            this.txt_UserPassword = new System.Windows.Forms.TextBox();
            this.txt_Username = new System.Windows.Forms.TextBox();
            this.nup_UserPort = new System.Windows.Forms.NumericUpDown();
            this.lnklbl_Size = new System.Windows.Forms.LinkLabel();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.tbctrl_Main.SuspendLayout();
            this.tp_Files.SuspendLayout();
            this.panel1.SuspendLayout();
            this.grp_FileInfo.SuspendLayout();
            this.flp_Hashes.SuspendLayout();
            this.pnl_Ed2k.SuspendLayout();
            this.pnl_Sha1.SuspendLayout();
            this.pnl_Md5.SuspendLayout();
            this.pnl_Crc.SuspendLayout();
            this.pnl_Tth.SuspendLayout();
            this.pnl_Aich.SuspendLayout();
            this.Settings.SuspendLayout();
            this.tp_Settings.SuspendLayout();
            this.grp_AVDump2GUISettings.SuspendLayout();
            this.grp_AVDump2Settings.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_CoreCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Timeout)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Retries)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_UserPort)).BeginInit();
            this.SuspendLayout();
            // 
            // tbctrl_Main
            // 
            this.tbctrl_Main.Controls.Add(this.tp_Files);
            this.tbctrl_Main.Controls.Add(this.tp_Settings);
            this.tbctrl_Main.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbctrl_Main.Location = new System.Drawing.Point(0, 0);
            this.tbctrl_Main.Name = "tbctrl_Main";
            this.tbctrl_Main.SelectedIndex = 0;
            this.tbctrl_Main.Size = new System.Drawing.Size(604, 264);
            this.tbctrl_Main.TabIndex = 0;
            // 
            // tp_Files
            // 
            this.tp_Files.Controls.Add(this.listView1);
            this.tp_Files.Controls.Add(this.panel1);
            this.tp_Files.Location = new System.Drawing.Point(4, 22);
            this.tp_Files.Name = "tp_Files";
            this.tp_Files.Padding = new System.Windows.Forms.Padding(3);
            this.tp_Files.Size = new System.Drawing.Size(596, 238);
            this.tp_Files.TabIndex = 0;
            this.tp_Files.Text = "Files";
            this.tp_Files.UseVisualStyleBackColor = true;
            // 
            // listView1
            // 
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.Location = new System.Drawing.Point(3, 3);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(590, 101);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            this.listView1.VirtualMode = true;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.grp_FileInfo);
            this.panel1.Controls.Add(this.lbl_Margin);
            this.panel1.Controls.Add(this.Settings);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(3, 104);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(590, 131);
            this.panel1.TabIndex = 2;
            // 
            // grp_FileInfo
            // 
            this.grp_FileInfo.Controls.Add(this.lnklbl_Size);
            this.grp_FileInfo.Controls.Add(this.lnklbl_GotoAniDB);
            this.grp_FileInfo.Controls.Add(this.lbl_Size);
            this.grp_FileInfo.Controls.Add(this.btn_CopyHashes);
            this.grp_FileInfo.Controls.Add(this.chcklst_CopyHashes);
            this.grp_FileInfo.Controls.Add(this.lbl_CopyAll);
            this.grp_FileInfo.Controls.Add(this.flp_Hashes);
            this.grp_FileInfo.Dock = System.Windows.Forms.DockStyle.Left;
            this.grp_FileInfo.Location = new System.Drawing.Point(161, 0);
            this.grp_FileInfo.Name = "grp_FileInfo";
            this.grp_FileInfo.Size = new System.Drawing.Size(426, 131);
            this.grp_FileInfo.TabIndex = 2;
            this.grp_FileInfo.TabStop = false;
            this.grp_FileInfo.Text = "File Info";
            // 
            // lnklbl_GotoAniDB
            // 
            this.lnklbl_GotoAniDB.AutoSize = true;
            this.lnklbl_GotoAniDB.Location = new System.Drawing.Point(357, 102);
            this.lnklbl_GotoAniDB.Name = "lnklbl_GotoAniDB";
            this.lnklbl_GotoAniDB.Size = new System.Drawing.Size(63, 13);
            this.lnklbl_GotoAniDB.TabIndex = 6;
            this.lnklbl_GotoAniDB.TabStop = true;
            this.lnklbl_GotoAniDB.Text = "Goto AniDB";
            // 
            // lbl_Size
            // 
            this.lbl_Size.AutoSize = true;
            this.lbl_Size.Location = new System.Drawing.Point(4, 102);
            this.lbl_Size.Name = "lbl_Size";
            this.lbl_Size.Size = new System.Drawing.Size(30, 13);
            this.lbl_Size.TabIndex = 4;
            this.lbl_Size.Text = "Size:";
            // 
            // btn_CopyHashes
            // 
            this.btn_CopyHashes.Location = new System.Drawing.Point(344, 79);
            this.btn_CopyHashes.Name = "btn_CopyHashes";
            this.btn_CopyHashes.Size = new System.Drawing.Size(79, 21);
            this.btn_CopyHashes.TabIndex = 3;
            this.btn_CopyHashes.Text = "Copy Hashes";
            this.btn_CopyHashes.UseVisualStyleBackColor = true;
            // 
            // chcklst_CopyHashes
            // 
            this.chcklst_CopyHashes.ColumnWidth = 46;
            this.chcklst_CopyHashes.FormattingEnabled = true;
            this.chcklst_CopyHashes.Items.AddRange(new object[] {
            "Ed2k",
            "Sha1",
            "Md5",
            "Crc",
            "Aich",
            "Tth"});
            this.chcklst_CopyHashes.Location = new System.Drawing.Point(59, 80);
            this.chcklst_CopyHashes.MultiColumn = true;
            this.chcklst_CopyHashes.Name = "chcklst_CopyHashes";
            this.chcklst_CopyHashes.Size = new System.Drawing.Size(282, 19);
            this.chcklst_CopyHashes.TabIndex = 2;
            this.chcklst_CopyHashes.Tag = "CopyHashes";
            // 
            // lbl_CopyAll
            // 
            this.lbl_CopyAll.AutoSize = true;
            this.lbl_CopyAll.Location = new System.Drawing.Point(4, 83);
            this.lbl_CopyAll.Name = "lbl_CopyAll";
            this.lbl_CopyAll.Size = new System.Drawing.Size(57, 13);
            this.lbl_CopyAll.TabIndex = 1;
            this.lbl_CopyAll.Text = "Copy All ...";
            // 
            // flp_Hashes
            // 
            this.flp_Hashes.AutoScroll = true;
            this.flp_Hashes.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flp_Hashes.Controls.Add(this.pnl_Ed2k);
            this.flp_Hashes.Controls.Add(this.pnl_Sha1);
            this.flp_Hashes.Controls.Add(this.pnl_Md5);
            this.flp_Hashes.Controls.Add(this.pnl_Crc);
            this.flp_Hashes.Controls.Add(this.pnl_Tth);
            this.flp_Hashes.Controls.Add(this.pnl_Aich);
            this.flp_Hashes.Controls.Add(this.lnklbl_CopyAll);
            this.flp_Hashes.Dock = System.Windows.Forms.DockStyle.Top;
            this.flp_Hashes.Location = new System.Drawing.Point(3, 16);
            this.flp_Hashes.Name = "flp_Hashes";
            this.flp_Hashes.Size = new System.Drawing.Size(420, 61);
            this.flp_Hashes.TabIndex = 0;
            // 
            // pnl_Ed2k
            // 
            this.pnl_Ed2k.Controls.Add(this.lbl_Ed2kHash);
            this.pnl_Ed2k.Controls.Add(this.lbl_Ed2k);
            this.pnl_Ed2k.Controls.Add(this.lnklbl_CopyEd2kHash);
            this.pnl_Ed2k.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnl_Ed2k.Location = new System.Drawing.Point(0, 0);
            this.pnl_Ed2k.Margin = new System.Windows.Forms.Padding(0);
            this.pnl_Ed2k.Name = "pnl_Ed2k";
            this.pnl_Ed2k.Size = new System.Drawing.Size(400, 15);
            this.pnl_Ed2k.TabIndex = 8;
            // 
            // lbl_Ed2kHash
            // 
            this.lbl_Ed2kHash.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_Ed2kHash.Location = new System.Drawing.Point(35, 0);
            this.lbl_Ed2kHash.Name = "lbl_Ed2kHash";
            this.lbl_Ed2kHash.Size = new System.Drawing.Size(332, 15);
            this.lbl_Ed2kHash.TabIndex = 1;
            this.lbl_Ed2kHash.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Ed2k
            // 
            this.lbl_Ed2k.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbl_Ed2k.Location = new System.Drawing.Point(0, 0);
            this.lbl_Ed2k.Name = "lbl_Ed2k";
            this.lbl_Ed2k.Size = new System.Drawing.Size(35, 15);
            this.lbl_Ed2k.TabIndex = 0;
            this.lbl_Ed2k.Text = "Ed2k:";
            this.lbl_Ed2k.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lnklbl_CopyEd2kHash
            // 
            this.lnklbl_CopyEd2kHash.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopyEd2kHash.Location = new System.Drawing.Point(367, 0);
            this.lnklbl_CopyEd2kHash.Name = "lnklbl_CopyEd2kHash";
            this.lnklbl_CopyEd2kHash.Size = new System.Drawing.Size(33, 15);
            this.lnklbl_CopyEd2kHash.TabIndex = 2;
            this.lnklbl_CopyEd2kHash.TabStop = true;
            this.lnklbl_CopyEd2kHash.Text = "Copy";
            // 
            // pnl_Sha1
            // 
            this.pnl_Sha1.Controls.Add(this.lbl_Sha1Hash);
            this.pnl_Sha1.Controls.Add(this.lbl_Sha1);
            this.pnl_Sha1.Controls.Add(this.lnklbl_CopySha1Hash);
            this.pnl_Sha1.Dock = System.Windows.Forms.DockStyle.Top;
            this.flp_Hashes.SetFlowBreak(this.pnl_Sha1, true);
            this.pnl_Sha1.Location = new System.Drawing.Point(0, 15);
            this.pnl_Sha1.Margin = new System.Windows.Forms.Padding(0);
            this.pnl_Sha1.Name = "pnl_Sha1";
            this.pnl_Sha1.Size = new System.Drawing.Size(400, 15);
            this.pnl_Sha1.TabIndex = 10;
            // 
            // lbl_Sha1Hash
            // 
            this.lbl_Sha1Hash.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_Sha1Hash.Location = new System.Drawing.Point(35, 0);
            this.lbl_Sha1Hash.Name = "lbl_Sha1Hash";
            this.lbl_Sha1Hash.Size = new System.Drawing.Size(332, 15);
            this.lbl_Sha1Hash.TabIndex = 1;
            this.lbl_Sha1Hash.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Sha1
            // 
            this.lbl_Sha1.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbl_Sha1.Location = new System.Drawing.Point(0, 0);
            this.lbl_Sha1.Name = "lbl_Sha1";
            this.lbl_Sha1.Size = new System.Drawing.Size(35, 15);
            this.lbl_Sha1.TabIndex = 0;
            this.lbl_Sha1.Text = "Sha1:";
            this.lbl_Sha1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lnklbl_CopySha1Hash
            // 
            this.lnklbl_CopySha1Hash.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopySha1Hash.Location = new System.Drawing.Point(367, 0);
            this.lnklbl_CopySha1Hash.Name = "lnklbl_CopySha1Hash";
            this.lnklbl_CopySha1Hash.Size = new System.Drawing.Size(33, 15);
            this.lnklbl_CopySha1Hash.TabIndex = 3;
            this.lnklbl_CopySha1Hash.TabStop = true;
            this.lnklbl_CopySha1Hash.Text = "Copy";
            // 
            // pnl_Md5
            // 
            this.pnl_Md5.Controls.Add(this.lbl_Md5Hash);
            this.pnl_Md5.Controls.Add(this.lbl_Md5);
            this.pnl_Md5.Controls.Add(this.lnklbl_CopyMd5Hash);
            this.pnl_Md5.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnl_Md5.Location = new System.Drawing.Point(0, 30);
            this.pnl_Md5.Margin = new System.Windows.Forms.Padding(0);
            this.pnl_Md5.Name = "pnl_Md5";
            this.pnl_Md5.Size = new System.Drawing.Size(400, 15);
            this.pnl_Md5.TabIndex = 9;
            // 
            // lbl_Md5Hash
            // 
            this.lbl_Md5Hash.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_Md5Hash.Location = new System.Drawing.Point(35, 0);
            this.lbl_Md5Hash.Name = "lbl_Md5Hash";
            this.lbl_Md5Hash.Size = new System.Drawing.Size(332, 15);
            this.lbl_Md5Hash.TabIndex = 1;
            this.lbl_Md5Hash.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Md5
            // 
            this.lbl_Md5.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbl_Md5.Location = new System.Drawing.Point(0, 0);
            this.lbl_Md5.Name = "lbl_Md5";
            this.lbl_Md5.Size = new System.Drawing.Size(35, 15);
            this.lbl_Md5.TabIndex = 0;
            this.lbl_Md5.Text = "Md5:";
            this.lbl_Md5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lnklbl_CopyMd5Hash
            // 
            this.lnklbl_CopyMd5Hash.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopyMd5Hash.Location = new System.Drawing.Point(367, 0);
            this.lnklbl_CopyMd5Hash.Name = "lnklbl_CopyMd5Hash";
            this.lnklbl_CopyMd5Hash.Size = new System.Drawing.Size(33, 15);
            this.lnklbl_CopyMd5Hash.TabIndex = 3;
            this.lnklbl_CopyMd5Hash.TabStop = true;
            this.lnklbl_CopyMd5Hash.Text = "Copy";
            // 
            // pnl_Crc
            // 
            this.pnl_Crc.Controls.Add(this.lbl_CrcHash);
            this.pnl_Crc.Controls.Add(this.lbl_Crc);
            this.pnl_Crc.Controls.Add(this.lnklbl_CopyCrcHash);
            this.pnl_Crc.Dock = System.Windows.Forms.DockStyle.Top;
            this.flp_Hashes.SetFlowBreak(this.pnl_Crc, true);
            this.pnl_Crc.Location = new System.Drawing.Point(0, 45);
            this.pnl_Crc.Margin = new System.Windows.Forms.Padding(0);
            this.pnl_Crc.Name = "pnl_Crc";
            this.pnl_Crc.Size = new System.Drawing.Size(400, 15);
            this.pnl_Crc.TabIndex = 12;
            // 
            // lbl_CrcHash
            // 
            this.lbl_CrcHash.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_CrcHash.Location = new System.Drawing.Point(35, 0);
            this.lbl_CrcHash.Name = "lbl_CrcHash";
            this.lbl_CrcHash.Size = new System.Drawing.Size(332, 15);
            this.lbl_CrcHash.TabIndex = 1;
            this.lbl_CrcHash.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Crc
            // 
            this.lbl_Crc.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbl_Crc.Location = new System.Drawing.Point(0, 0);
            this.lbl_Crc.Name = "lbl_Crc";
            this.lbl_Crc.Size = new System.Drawing.Size(35, 15);
            this.lbl_Crc.TabIndex = 0;
            this.lbl_Crc.Text = "Crc:";
            this.lbl_Crc.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lnklbl_CopyCrcHash
            // 
            this.lnklbl_CopyCrcHash.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopyCrcHash.Location = new System.Drawing.Point(367, 0);
            this.lnklbl_CopyCrcHash.Name = "lnklbl_CopyCrcHash";
            this.lnklbl_CopyCrcHash.Size = new System.Drawing.Size(33, 15);
            this.lnklbl_CopyCrcHash.TabIndex = 3;
            this.lnklbl_CopyCrcHash.TabStop = true;
            this.lnklbl_CopyCrcHash.Text = "Copy";
            // 
            // pnl_Tth
            // 
            this.pnl_Tth.Controls.Add(this.lbl_TthHash);
            this.pnl_Tth.Controls.Add(this.lbl_Tth);
            this.pnl_Tth.Controls.Add(this.lnklbl_CopyTthHash);
            this.pnl_Tth.Dock = System.Windows.Forms.DockStyle.Top;
            this.flp_Hashes.SetFlowBreak(this.pnl_Tth, true);
            this.pnl_Tth.Location = new System.Drawing.Point(0, 60);
            this.pnl_Tth.Margin = new System.Windows.Forms.Padding(0);
            this.pnl_Tth.Name = "pnl_Tth";
            this.pnl_Tth.Size = new System.Drawing.Size(400, 15);
            this.pnl_Tth.TabIndex = 13;
            // 
            // lbl_TthHash
            // 
            this.lbl_TthHash.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_TthHash.Location = new System.Drawing.Point(35, 0);
            this.lbl_TthHash.Name = "lbl_TthHash";
            this.lbl_TthHash.Size = new System.Drawing.Size(332, 15);
            this.lbl_TthHash.TabIndex = 1;
            this.lbl_TthHash.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Tth
            // 
            this.lbl_Tth.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbl_Tth.Location = new System.Drawing.Point(0, 0);
            this.lbl_Tth.Name = "lbl_Tth";
            this.lbl_Tth.Size = new System.Drawing.Size(35, 15);
            this.lbl_Tth.TabIndex = 0;
            this.lbl_Tth.Text = "Tth:";
            this.lbl_Tth.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lnklbl_CopyTthHash
            // 
            this.lnklbl_CopyTthHash.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopyTthHash.Location = new System.Drawing.Point(367, 0);
            this.lnklbl_CopyTthHash.Name = "lnklbl_CopyTthHash";
            this.lnklbl_CopyTthHash.Size = new System.Drawing.Size(33, 15);
            this.lnklbl_CopyTthHash.TabIndex = 3;
            this.lnklbl_CopyTthHash.TabStop = true;
            this.lnklbl_CopyTthHash.Text = "Copy";
            // 
            // pnl_Aich
            // 
            this.pnl_Aich.Controls.Add(this.lbl_AichHash);
            this.pnl_Aich.Controls.Add(this.Aich);
            this.pnl_Aich.Controls.Add(this.lnklbl_CopyAichHash);
            this.pnl_Aich.Dock = System.Windows.Forms.DockStyle.Top;
            this.flp_Hashes.SetFlowBreak(this.pnl_Aich, true);
            this.pnl_Aich.Location = new System.Drawing.Point(0, 75);
            this.pnl_Aich.Margin = new System.Windows.Forms.Padding(0);
            this.pnl_Aich.Name = "pnl_Aich";
            this.pnl_Aich.Size = new System.Drawing.Size(400, 15);
            this.pnl_Aich.TabIndex = 11;
            // 
            // lbl_AichHash
            // 
            this.lbl_AichHash.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbl_AichHash.Location = new System.Drawing.Point(35, 0);
            this.lbl_AichHash.Name = "lbl_AichHash";
            this.lbl_AichHash.Size = new System.Drawing.Size(332, 15);
            this.lbl_AichHash.TabIndex = 1;
            this.lbl_AichHash.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // Aich
            // 
            this.Aich.Dock = System.Windows.Forms.DockStyle.Left;
            this.Aich.Location = new System.Drawing.Point(0, 0);
            this.Aich.Name = "Aich";
            this.Aich.Size = new System.Drawing.Size(35, 15);
            this.Aich.TabIndex = 0;
            this.Aich.Text = "Aich:";
            this.Aich.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lnklbl_CopyAichHash
            // 
            this.lnklbl_CopyAichHash.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopyAichHash.Location = new System.Drawing.Point(367, 0);
            this.lnklbl_CopyAichHash.Name = "lnklbl_CopyAichHash";
            this.lnklbl_CopyAichHash.Size = new System.Drawing.Size(33, 15);
            this.lnklbl_CopyAichHash.TabIndex = 3;
            this.lnklbl_CopyAichHash.TabStop = true;
            this.lnklbl_CopyAichHash.Text = "Copy";
            // 
            // lnklbl_CopyAll
            // 
            this.lnklbl_CopyAll.Dock = System.Windows.Forms.DockStyle.Right;
            this.lnklbl_CopyAll.Location = new System.Drawing.Point(0, 90);
            this.lnklbl_CopyAll.Margin = new System.Windows.Forms.Padding(0);
            this.lnklbl_CopyAll.MinimumSize = new System.Drawing.Size(400, 14);
            this.lnklbl_CopyAll.Name = "lnklbl_CopyAll";
            this.lnklbl_CopyAll.Size = new System.Drawing.Size(400, 14);
            this.lnklbl_CopyAll.TabIndex = 14;
            this.lnklbl_CopyAll.TabStop = true;
            this.lnklbl_CopyAll.Text = "Copy All";
            this.lnklbl_CopyAll.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lbl_Margin
            // 
            this.lbl_Margin.Dock = System.Windows.Forms.DockStyle.Left;
            this.lbl_Margin.Location = new System.Drawing.Point(153, 0);
            this.lbl_Margin.Name = "lbl_Margin";
            this.lbl_Margin.Size = new System.Drawing.Size(8, 131);
            this.lbl_Margin.TabIndex = 3;
            // 
            // Settings
            // 
            this.Settings.Controls.Add(this.checkBox1);
            this.Settings.Controls.Add(this.chck_ACreq);
            this.Settings.Controls.Add(this.chcklst_Hashes);
            this.Settings.Dock = System.Windows.Forms.DockStyle.Left;
            this.Settings.Location = new System.Drawing.Point(0, 0);
            this.Settings.Name = "Settings";
            this.Settings.Size = new System.Drawing.Size(153, 131);
            this.Settings.TabIndex = 1;
            this.Settings.TabStop = false;
            this.Settings.Text = "Settings";
            // 
            // chcklst_Hashes
            // 
            this.chcklst_Hashes.Dock = System.Windows.Forms.DockStyle.Left;
            this.chcklst_Hashes.FormattingEnabled = true;
            this.chcklst_Hashes.Items.AddRange(new object[] {
            "Ed2k",
            "Sha1",
            "Md5",
            "Crc",
            "Aich",
            "Tth"});
            this.chcklst_Hashes.Location = new System.Drawing.Point(3, 16);
            this.chcklst_Hashes.Name = "chcklst_Hashes";
            this.chcklst_Hashes.Size = new System.Drawing.Size(57, 109);
            this.chcklst_Hashes.TabIndex = 1;
            this.chcklst_Hashes.Tag = "CalcHashes";
            // 
            // tp_Settings
            // 
            this.tp_Settings.Controls.Add(this.grp_AVDump2GUISettings);
            this.tp_Settings.Controls.Add(this.grp_AVDump2Settings);
            this.tp_Settings.Controls.Add(this.groupBox1);
            this.tp_Settings.Location = new System.Drawing.Point(4, 22);
            this.tp_Settings.Name = "tp_Settings";
            this.tp_Settings.Padding = new System.Windows.Forms.Padding(3);
            this.tp_Settings.Size = new System.Drawing.Size(596, 238);
            this.tp_Settings.TabIndex = 1;
            this.tp_Settings.Text = "Settings";
            this.tp_Settings.UseVisualStyleBackColor = true;
            // 
            // grp_AVDump2GUISettings
            // 
            this.grp_AVDump2GUISettings.Controls.Add(this.lbl_UseColumns);
            this.grp_AVDump2GUISettings.Controls.Add(this.chcklst_Columns);
            this.grp_AVDump2GUISettings.Location = new System.Drawing.Point(245, 6);
            this.grp_AVDump2GUISettings.Name = "grp_AVDump2GUISettings";
            this.grp_AVDump2GUISettings.Size = new System.Drawing.Size(130, 117);
            this.grp_AVDump2GUISettings.TabIndex = 16;
            this.grp_AVDump2GUISettings.TabStop = false;
            this.grp_AVDump2GUISettings.Text = "AVDump2GUI Settings";
            // 
            // lbl_UseColumns
            // 
            this.lbl_UseColumns.AutoSize = true;
            this.lbl_UseColumns.Location = new System.Drawing.Point(6, 16);
            this.lbl_UseColumns.Name = "lbl_UseColumns";
            this.lbl_UseColumns.Size = new System.Drawing.Size(72, 13);
            this.lbl_UseColumns.TabIndex = 1;
            this.lbl_UseColumns.Text = "Use Columns:";
            // 
            // chcklst_Columns
            // 
            this.chcklst_Columns.FormattingEnabled = true;
            this.chcklst_Columns.Items.AddRange(new object[] {
            "Name",
            "Size",
            "Ed2k",
            "Crc",
            "Md5",
            "Sha1",
            "Tth",
            "Aich",
            "ACreq"});
            this.chcklst_Columns.Location = new System.Drawing.Point(6, 32);
            this.chcklst_Columns.Name = "chcklst_Columns";
            this.chcklst_Columns.Size = new System.Drawing.Size(73, 79);
            this.chcklst_Columns.TabIndex = 0;
            this.chcklst_Columns.Tag = "UseColumns";
            // 
            // grp_AVDump2Settings
            // 
            this.grp_AVDump2Settings.Controls.Add(this.label1);
            this.grp_AVDump2Settings.Controls.Add(this.nup_BlockCount);
            this.grp_AVDump2Settings.Controls.Add(this.nup_CoreCount);
            this.grp_AVDump2Settings.Controls.Add(this.chck_IntelliProc);
            this.grp_AVDump2Settings.Controls.Add(this.lbl_BlockSettings);
            this.grp_AVDump2Settings.Controls.Add(this.nup_BlockSize);
            this.grp_AVDump2Settings.Controls.Add(this.lbl_ACreqSettings);
            this.grp_AVDump2Settings.Controls.Add(this.nup_Timeout);
            this.grp_AVDump2Settings.Controls.Add(this.nup_Retries);
            this.grp_AVDump2Settings.Location = new System.Drawing.Point(6, 132);
            this.grp_AVDump2Settings.Name = "grp_AVDump2Settings";
            this.grp_AVDump2Settings.Size = new System.Drawing.Size(233, 96);
            this.grp_AVDump2Settings.TabIndex = 15;
            this.grp_AVDump2Settings.TabStop = false;
            this.grp_AVDump2Settings.Text = "AVDump2 Settings";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(91, 70);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(103, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Assume Core Count:";
            // 
            // lbl_BlockSettings
            // 
            this.lbl_BlockSettings.AutoSize = true;
            this.lbl_BlockSettings.Location = new System.Drawing.Point(6, 18);
            this.lbl_BlockSettings.Name = "lbl_BlockSettings";
            this.lbl_BlockSettings.Size = new System.Drawing.Size(115, 13);
            this.lbl_BlockSettings.TabIndex = 5;
            this.lbl_BlockSettings.Text = "Blocksize, Blockcount:";
            // 
            // lbl_ACreqSettings
            // 
            this.lbl_ACreqSettings.AutoSize = true;
            this.lbl_ACreqSettings.Location = new System.Drawing.Point(8, 44);
            this.lbl_ACreqSettings.Name = "lbl_ACreqSettings";
            this.lbl_ACreqSettings.Size = new System.Drawing.Size(113, 13);
            this.lbl_ACreqSettings.TabIndex = 8;
            this.lbl_ACreqSettings.Text = "Timeout(sec) , Retires:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lbl_UserPassword);
            this.groupBox1.Controls.Add(this.txt_APIHost);
            this.groupBox1.Controls.Add(this.lbl_Username);
            this.groupBox1.Controls.Add(this.txt_UserPassword);
            this.groupBox1.Controls.Add(this.lbl_APIHost);
            this.groupBox1.Controls.Add(this.txt_Username);
            this.groupBox1.Controls.Add(this.lbl_UserPort);
            this.groupBox1.Controls.Add(this.nup_UserPort);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(233, 120);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "AniDB Settings";
            // 
            // lbl_UserPassword
            // 
            this.lbl_UserPassword.AutoSize = true;
            this.lbl_UserPassword.Location = new System.Drawing.Point(124, 73);
            this.lbl_UserPassword.Name = "lbl_UserPassword";
            this.lbl_UserPassword.Size = new System.Drawing.Size(56, 13);
            this.lbl_UserPassword.TabIndex = 19;
            this.lbl_UserPassword.Text = "Password:";
            // 
            // lbl_Username
            // 
            this.lbl_Username.AutoSize = true;
            this.lbl_Username.Location = new System.Drawing.Point(6, 73);
            this.lbl_Username.Name = "lbl_Username";
            this.lbl_Username.Size = new System.Drawing.Size(58, 13);
            this.lbl_Username.TabIndex = 18;
            this.lbl_Username.Text = "Username:";
            // 
            // lbl_APIHost
            // 
            this.lbl_APIHost.AutoSize = true;
            this.lbl_APIHost.Location = new System.Drawing.Point(8, 20);
            this.lbl_APIHost.Name = "lbl_APIHost";
            this.lbl_APIHost.Size = new System.Drawing.Size(52, 13);
            this.lbl_APIHost.TabIndex = 1;
            this.lbl_APIHost.Text = "API Host:";
            // 
            // lbl_UserPort
            // 
            this.lbl_UserPort.AutoSize = true;
            this.lbl_UserPort.Location = new System.Drawing.Point(6, 45);
            this.lbl_UserPort.Name = "lbl_UserPort";
            this.lbl_UserPort.Size = new System.Drawing.Size(54, 13);
            this.lbl_UserPort.TabIndex = 3;
            this.lbl_UserPort.Text = "User Port:";
            // 
            // chck_ACreq
            // 
            this.chck_ACreq.AutoSize = true;
            this.chck_ACreq.Checked = global::AVDump2GUI.Properties.Settings.Default.MakeAcreq;
            this.chck_ACreq.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chck_ACreq.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::AVDump2GUI.Properties.Settings.Default, "MakeAcreq", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chck_ACreq.Location = new System.Drawing.Point(65, 15);
            this.chck_ACreq.Name = "chck_ACreq";
            this.chck_ACreq.Size = new System.Drawing.Size(85, 17);
            this.chck_ACreq.TabIndex = 1;
            this.chck_ACreq.Tag = "MakeAcreq";
            this.chck_ACreq.Text = "Make ACreq";
            this.chck_ACreq.UseVisualStyleBackColor = true;
            // 
            // nup_BlockCount
            // 
            this.nup_BlockCount.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::AVDump2GUI.Properties.Settings.Default, "BlockCount", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nup_BlockCount.Location = new System.Drawing.Point(187, 16);
            this.nup_BlockCount.Maximum = new decimal(new int[] {
            512,
            0,
            0,
            0});
            this.nup_BlockCount.Minimum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.nup_BlockCount.Name = "nup_BlockCount";
            this.nup_BlockCount.Size = new System.Drawing.Size(39, 20);
            this.nup_BlockCount.TabIndex = 7;
            this.nup_BlockCount.Tag = "BlockCount";
            this.nup_BlockCount.Value = global::AVDump2GUI.Properties.Settings.Default.BlockCount;
            // 
            // nup_CoreCount
            // 
            this.nup_CoreCount.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::AVDump2GUI.Properties.Settings.Default, "CoreCount", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nup_CoreCount.Location = new System.Drawing.Point(194, 68);
            this.nup_CoreCount.Maximum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.nup_CoreCount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nup_CoreCount.Name = "nup_CoreCount";
            this.nup_CoreCount.Size = new System.Drawing.Size(32, 20);
            this.nup_CoreCount.TabIndex = 12;
            this.nup_CoreCount.Tag = "CoreCount";
            this.nup_CoreCount.Value = global::AVDump2GUI.Properties.Settings.Default.CoreCount;
            // 
            // chck_IntelliProc
            // 
            this.chck_IntelliProc.AutoSize = true;
            this.chck_IntelliProc.Checked = global::AVDump2GUI.Properties.Settings.Default.IntelliProc;
            this.chck_IntelliProc.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chck_IntelliProc.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::AVDump2GUI.Properties.Settings.Default, "IntelliProc", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chck_IntelliProc.Location = new System.Drawing.Point(6, 69);
            this.chck_IntelliProc.Name = "chck_IntelliProc";
            this.chck_IntelliProc.Size = new System.Drawing.Size(81, 17);
            this.chck_IntelliProc.TabIndex = 11;
            this.chck_IntelliProc.Tag = "IntelliProc";
            this.chck_IntelliProc.Text = "IntelliProc©";
            this.chck_IntelliProc.UseVisualStyleBackColor = true;
            // 
            // nup_BlockSize
            // 
            this.nup_BlockSize.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::AVDump2GUI.Properties.Settings.Default, "BlockSize", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nup_BlockSize.Location = new System.Drawing.Point(127, 16);
            this.nup_BlockSize.Maximum = new decimal(new int[] {
            10240,
            0,
            0,
            0});
            this.nup_BlockSize.Minimum = new decimal(new int[] {
            128,
            0,
            0,
            0});
            this.nup_BlockSize.Name = "nup_BlockSize";
            this.nup_BlockSize.Size = new System.Drawing.Size(54, 20);
            this.nup_BlockSize.TabIndex = 6;
            this.nup_BlockSize.Tag = "BlockSize";
            this.nup_BlockSize.Value = global::AVDump2GUI.Properties.Settings.Default.BlockSize;
            // 
            // nup_Timeout
            // 
            this.nup_Timeout.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::AVDump2GUI.Properties.Settings.Default, "TimeoutSeconds", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nup_Timeout.Location = new System.Drawing.Point(127, 42);
            this.nup_Timeout.Maximum = new decimal(new int[] {
            30,
            0,
            0,
            0});
            this.nup_Timeout.Minimum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.nup_Timeout.Name = "nup_Timeout";
            this.nup_Timeout.Size = new System.Drawing.Size(54, 20);
            this.nup_Timeout.TabIndex = 9;
            this.nup_Timeout.Tag = "TimeoutSeconds";
            this.nup_Timeout.Value = global::AVDump2GUI.Properties.Settings.Default.TimeoutSeconds;
            this.nup_Timeout.ValueChanged += new System.EventHandler(this.nup_Timeout_ValueChanged);
            // 
            // nup_Retries
            // 
            this.nup_Retries.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::AVDump2GUI.Properties.Settings.Default, "MaxRetryCount", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nup_Retries.Location = new System.Drawing.Point(194, 42);
            this.nup_Retries.Maximum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.nup_Retries.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.nup_Retries.Name = "nup_Retries";
            this.nup_Retries.Size = new System.Drawing.Size(32, 20);
            this.nup_Retries.TabIndex = 10;
            this.nup_Retries.Tag = "MaxRetryCount";
            this.nup_Retries.Value = global::AVDump2GUI.Properties.Settings.Default.MaxRetryCount;
            // 
            // txt_APIHost
            // 
            this.txt_APIHost.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::AVDump2GUI.Properties.Settings.Default, "AniDBHost", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.txt_APIHost.Location = new System.Drawing.Point(66, 17);
            this.txt_APIHost.Name = "txt_APIHost";
            this.txt_APIHost.Size = new System.Drawing.Size(125, 20);
            this.txt_APIHost.TabIndex = 0;
            this.txt_APIHost.Tag = "AniDBHost";
            this.txt_APIHost.Text = global::AVDump2GUI.Properties.Settings.Default.AniDBHost;
            // 
            // txt_UserPassword
            // 
            this.txt_UserPassword.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::AVDump2GUI.Properties.Settings.Default, "UserPassword", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.txt_UserPassword.Location = new System.Drawing.Point(127, 89);
            this.txt_UserPassword.Name = "txt_UserPassword";
            this.txt_UserPassword.Size = new System.Drawing.Size(98, 20);
            this.txt_UserPassword.TabIndex = 17;
            this.txt_UserPassword.Tag = "UserPassword";
            this.txt_UserPassword.Text = global::AVDump2GUI.Properties.Settings.Default.UserPassword;
            // 
            // txt_Username
            // 
            this.txt_Username.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::AVDump2GUI.Properties.Settings.Default, "Username", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.txt_Username.Location = new System.Drawing.Point(9, 89);
            this.txt_Username.Name = "txt_Username";
            this.txt_Username.Size = new System.Drawing.Size(98, 20);
            this.txt_Username.TabIndex = 16;
            this.txt_Username.Tag = "Username";
            this.txt_Username.Text = global::AVDump2GUI.Properties.Settings.Default.Username;
            // 
            // nup_UserPort
            // 
            this.nup_UserPort.DataBindings.Add(new System.Windows.Forms.Binding("Value", global::AVDump2GUI.Properties.Settings.Default, "UserPort", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.nup_UserPort.Location = new System.Drawing.Point(66, 43);
            this.nup_UserPort.Maximum = new decimal(new int[] {
            65000,
            0,
            0,
            0});
            this.nup_UserPort.Minimum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nup_UserPort.Name = "nup_UserPort";
            this.nup_UserPort.Size = new System.Drawing.Size(54, 20);
            this.nup_UserPort.TabIndex = 4;
            this.nup_UserPort.Tag = "UserPort";
            this.nup_UserPort.Value = global::AVDump2GUI.Properties.Settings.Default.UserPort;
            // 
            // lnklbl_Size
            // 
            this.lnklbl_Size.Location = new System.Drawing.Point(33, 102);
            this.lnklbl_Size.Name = "lnklbl_Size";
            this.lnklbl_Size.Size = new System.Drawing.Size(110, 15);
            this.lnklbl_Size.TabIndex = 7;
            this.lnklbl_Size.TabStop = true;
            this.lnklbl_Size.Text = "00000000000000000";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = global::AVDump2GUI.Properties.Settings.Default.MakeAcreq;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::AVDump2GUI.Properties.Settings.Default, "MakeAcreq", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox1.Location = new System.Drawing.Point(65, 35);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(79, 17);
            this.checkBox1.TabIndex = 2;
            this.checkBox1.Tag = "MakeAcreq";
            this.checkBox1.Text = "Gather Info";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // GUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(604, 264);
            this.Controls.Add(this.tbctrl_Main);
            this.MinimumSize = new System.Drawing.Size(620, 300);
            this.Name = "GUI";
            this.Text = "AVDump2 GUI";
            this.tbctrl_Main.ResumeLayout(false);
            this.tp_Files.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.grp_FileInfo.ResumeLayout(false);
            this.grp_FileInfo.PerformLayout();
            this.flp_Hashes.ResumeLayout(false);
            this.pnl_Ed2k.ResumeLayout(false);
            this.pnl_Sha1.ResumeLayout(false);
            this.pnl_Md5.ResumeLayout(false);
            this.pnl_Crc.ResumeLayout(false);
            this.pnl_Tth.ResumeLayout(false);
            this.pnl_Aich.ResumeLayout(false);
            this.Settings.ResumeLayout(false);
            this.Settings.PerformLayout();
            this.tp_Settings.ResumeLayout(false);
            this.grp_AVDump2GUISettings.ResumeLayout(false);
            this.grp_AVDump2GUISettings.PerformLayout();
            this.grp_AVDump2Settings.ResumeLayout(false);
            this.grp_AVDump2Settings.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_CoreCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Timeout)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Retries)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_UserPort)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tbctrl_Main;
        private System.Windows.Forms.TabPage tp_Files;
        private System.Windows.Forms.TabPage tp_Settings;
        private System.Windows.Forms.Label lbl_APIHost;
        private System.Windows.Forms.TextBox txt_APIHost;
        private System.Windows.Forms.Label lbl_UserPort;
        private System.Windows.Forms.NumericUpDown nup_UserPort;
        private System.Windows.Forms.NumericUpDown nup_BlockCount;
        private System.Windows.Forms.NumericUpDown nup_BlockSize;
        private System.Windows.Forms.Label lbl_BlockSettings;
        private System.Windows.Forms.NumericUpDown nup_Retries;
        private System.Windows.Forms.NumericUpDown nup_Timeout;
        private System.Windows.Forms.Label lbl_ACreqSettings;
        private System.Windows.Forms.CheckBox chck_IntelliProc;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nup_CoreCount;
        private System.Windows.Forms.GroupBox grp_AVDump2Settings;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.GroupBox Settings;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.GroupBox grp_FileInfo;
        private System.Windows.Forms.FlowLayoutPanel flp_Hashes;
        private System.Windows.Forms.Panel pnl_Tth;
        private System.Windows.Forms.Label lbl_TthHash;
        private System.Windows.Forms.Label lbl_Tth;
        private System.Windows.Forms.Panel pnl_Crc;
        private System.Windows.Forms.Label lbl_CrcHash;
        private System.Windows.Forms.Label lbl_Crc;
        private System.Windows.Forms.Panel pnl_Aich;
        private System.Windows.Forms.Label lbl_AichHash;
        private System.Windows.Forms.Label Aich;
        private System.Windows.Forms.Panel pnl_Sha1;
        private System.Windows.Forms.Label lbl_Sha1Hash;
        private System.Windows.Forms.Label lbl_Sha1;
        private System.Windows.Forms.Panel pnl_Md5;
        private System.Windows.Forms.Label lbl_Md5Hash;
        private System.Windows.Forms.Label lbl_Md5;
        private System.Windows.Forms.Panel pnl_Ed2k;
        private System.Windows.Forms.Label lbl_Ed2kHash;
        private System.Windows.Forms.Label lbl_Ed2k;
        private System.Windows.Forms.Label lbl_Margin;
        private System.Windows.Forms.LinkLabel lnklbl_CopyEd2kHash;
        private System.Windows.Forms.LinkLabel lnklbl_CopySha1Hash;
        private System.Windows.Forms.LinkLabel lnklbl_CopyMd5Hash;
        private System.Windows.Forms.LinkLabel lnklbl_CopyCrcHash;
        private System.Windows.Forms.LinkLabel lnklbl_CopyTthHash;
        private System.Windows.Forms.LinkLabel lnklbl_CopyAichHash;
        private System.Windows.Forms.LinkLabel lnklbl_CopyAll;
        private System.Windows.Forms.CheckedListBox chcklst_Hashes;
        private System.Windows.Forms.CheckBox chck_ACreq;
        private System.Windows.Forms.Label lbl_UserPassword;
        private System.Windows.Forms.Label lbl_Username;
        private System.Windows.Forms.TextBox txt_UserPassword;
        private System.Windows.Forms.TextBox txt_Username;
        private System.Windows.Forms.CheckedListBox chcklst_CopyHashes;
        private System.Windows.Forms.Label lbl_CopyAll;
        private System.Windows.Forms.Label lbl_Size;
        private System.Windows.Forms.Button btn_CopyHashes;
        private System.Windows.Forms.LinkLabel lnklbl_GotoAniDB;
        private System.Windows.Forms.GroupBox grp_AVDump2GUISettings;
        private System.Windows.Forms.Label lbl_UseColumns;
        private System.Windows.Forms.CheckedListBox chcklst_Columns;
        private System.Windows.Forms.LinkLabel lnklbl_Size;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}

