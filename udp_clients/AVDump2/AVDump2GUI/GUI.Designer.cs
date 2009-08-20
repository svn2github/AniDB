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
            this.tp_Settings = new System.Windows.Forms.TabPage();
            this.grp_AVDump2GUISettings = new System.Windows.Forms.GroupBox();
            this.lbl_UseColumns = new System.Windows.Forms.Label();
            this.chcklst_Columns = new System.Windows.Forms.CheckedListBox();
            this.grp_AVDump2Settings = new System.Windows.Forms.GroupBox();
            this.nup_BlockCount = new System.Windows.Forms.NumericUpDown();
            this.lbl_BlockSettings = new System.Windows.Forms.Label();
            this.nup_BlockSize = new System.Windows.Forms.NumericUpDown();
            this.lbl_ACreqSettings = new System.Windows.Forms.Label();
            this.nup_Timeout = new System.Windows.Forms.NumericUpDown();
            this.nup_Retries = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lbl_UserPassword = new System.Windows.Forms.Label();
            this.txt_APIHost = new System.Windows.Forms.TextBox();
            this.lbl_Username = new System.Windows.Forms.Label();
            this.txt_UserPassword = new System.Windows.Forms.TextBox();
            this.lbl_APIHost = new System.Windows.Forms.Label();
            this.txt_Username = new System.Windows.Forms.TextBox();
            this.lbl_UserPort = new System.Windows.Forms.Label();
            this.nup_UserPort = new System.Windows.Forms.NumericUpDown();
            this.pnl_Progress = new System.Windows.Forms.Panel();
            this.chcklst_Hashes = new System.Windows.Forms.CheckedListBox();
            this.lbl_CalcHashes = new System.Windows.Forms.Label();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.chck_ACreq = new System.Windows.Forms.CheckBox();
            this.grp_Tagging = new System.Windows.Forms.GroupBox();
            this.tbctrl_Main.SuspendLayout();
            this.tp_Files.SuspendLayout();
            this.tp_Settings.SuspendLayout();
            this.grp_AVDump2GUISettings.SuspendLayout();
            this.grp_AVDump2Settings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Timeout)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Retries)).BeginInit();
            this.groupBox1.SuspendLayout();
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
            this.tbctrl_Main.Size = new System.Drawing.Size(604, 310);
            this.tbctrl_Main.TabIndex = 0;
            // 
            // tp_Files
            // 
            this.tp_Files.Controls.Add(this.listView1);
            this.tp_Files.Controls.Add(this.pnl_Progress);
            this.tp_Files.Location = new System.Drawing.Point(4, 22);
            this.tp_Files.Name = "tp_Files";
            this.tp_Files.Padding = new System.Windows.Forms.Padding(3);
            this.tp_Files.Size = new System.Drawing.Size(596, 284);
            this.tp_Files.TabIndex = 0;
            this.tp_Files.Text = "Files";
            this.tp_Files.UseVisualStyleBackColor = true;
            // 
            // listView1
            // 
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.Location = new System.Drawing.Point(3, 3);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(590, 255);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            this.listView1.VirtualMode = true;
            // 
            // tp_Settings
            // 
            this.tp_Settings.Controls.Add(this.grp_Tagging);
            this.tp_Settings.Controls.Add(this.grp_AVDump2GUISettings);
            this.tp_Settings.Controls.Add(this.grp_AVDump2Settings);
            this.tp_Settings.Controls.Add(this.groupBox1);
            this.tp_Settings.Location = new System.Drawing.Point(4, 22);
            this.tp_Settings.Name = "tp_Settings";
            this.tp_Settings.Padding = new System.Windows.Forms.Padding(3);
            this.tp_Settings.Size = new System.Drawing.Size(596, 284);
            this.tp_Settings.TabIndex = 1;
            this.tp_Settings.Text = "Settings";
            this.tp_Settings.UseVisualStyleBackColor = true;
            // 
            // grp_AVDump2GUISettings
            // 
            this.grp_AVDump2GUISettings.Controls.Add(this.lbl_UseColumns);
            this.grp_AVDump2GUISettings.Controls.Add(this.chcklst_Columns);
            this.grp_AVDump2GUISettings.Location = new System.Drawing.Point(6, 129);
            this.grp_AVDump2GUISettings.Name = "grp_AVDump2GUISettings";
            this.grp_AVDump2GUISettings.Size = new System.Drawing.Size(89, 117);
            this.grp_AVDump2GUISettings.TabIndex = 16;
            this.grp_AVDump2GUISettings.TabStop = false;
            this.grp_AVDump2GUISettings.Text = "AVDump2GUI";
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
            this.grp_AVDump2Settings.Controls.Add(this.lbl_CalcHashes);
            this.grp_AVDump2Settings.Controls.Add(this.checkBox1);
            this.grp_AVDump2Settings.Controls.Add(this.chcklst_Hashes);
            this.grp_AVDump2Settings.Controls.Add(this.chck_ACreq);
            this.grp_AVDump2Settings.Controls.Add(this.nup_BlockCount);
            this.grp_AVDump2Settings.Controls.Add(this.lbl_BlockSettings);
            this.grp_AVDump2Settings.Controls.Add(this.nup_BlockSize);
            this.grp_AVDump2Settings.Controls.Add(this.lbl_ACreqSettings);
            this.grp_AVDump2Settings.Controls.Add(this.nup_Timeout);
            this.grp_AVDump2Settings.Controls.Add(this.nup_Retries);
            this.grp_AVDump2Settings.Location = new System.Drawing.Point(245, 6);
            this.grp_AVDump2Settings.Name = "grp_AVDump2Settings";
            this.grp_AVDump2Settings.Size = new System.Drawing.Size(319, 117);
            this.grp_AVDump2Settings.TabIndex = 15;
            this.grp_AVDump2Settings.TabStop = false;
            this.grp_AVDump2Settings.Text = "AVDump2";
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
            // lbl_BlockSettings
            // 
            this.lbl_BlockSettings.AutoSize = true;
            this.lbl_BlockSettings.Location = new System.Drawing.Point(6, 18);
            this.lbl_BlockSettings.Name = "lbl_BlockSettings";
            this.lbl_BlockSettings.Size = new System.Drawing.Size(115, 13);
            this.lbl_BlockSettings.TabIndex = 5;
            this.lbl_BlockSettings.Text = "Blocksize, Blockcount:";
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
            // lbl_ACreqSettings
            // 
            this.lbl_ACreqSettings.AutoSize = true;
            this.lbl_ACreqSettings.Location = new System.Drawing.Point(8, 44);
            this.lbl_ACreqSettings.Name = "lbl_ACreqSettings";
            this.lbl_ACreqSettings.Size = new System.Drawing.Size(113, 13);
            this.lbl_ACreqSettings.TabIndex = 8;
            this.lbl_ACreqSettings.Text = "Timeout(sec) , Retires:";
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
            this.groupBox1.Size = new System.Drawing.Size(233, 117);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "AniDB";
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
            // lbl_Username
            // 
            this.lbl_Username.AutoSize = true;
            this.lbl_Username.Location = new System.Drawing.Point(6, 73);
            this.lbl_Username.Name = "lbl_Username";
            this.lbl_Username.Size = new System.Drawing.Size(58, 13);
            this.lbl_Username.TabIndex = 18;
            this.lbl_Username.Text = "Username:";
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
            // lbl_APIHost
            // 
            this.lbl_APIHost.AutoSize = true;
            this.lbl_APIHost.Location = new System.Drawing.Point(8, 20);
            this.lbl_APIHost.Name = "lbl_APIHost";
            this.lbl_APIHost.Size = new System.Drawing.Size(52, 13);
            this.lbl_APIHost.TabIndex = 1;
            this.lbl_APIHost.Text = "API Host:";
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
            // lbl_UserPort
            // 
            this.lbl_UserPort.AutoSize = true;
            this.lbl_UserPort.Location = new System.Drawing.Point(6, 45);
            this.lbl_UserPort.Name = "lbl_UserPort";
            this.lbl_UserPort.Size = new System.Drawing.Size(54, 13);
            this.lbl_UserPort.TabIndex = 3;
            this.lbl_UserPort.Text = "User Port:";
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
            // pnl_Progress
            // 
            this.pnl_Progress.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnl_Progress.Location = new System.Drawing.Point(3, 258);
            this.pnl_Progress.Name = "pnl_Progress";
            this.pnl_Progress.Size = new System.Drawing.Size(590, 23);
            this.pnl_Progress.TabIndex = 3;
            this.pnl_Progress.Visible = false;
            // 
            // chcklst_Hashes
            // 
            this.chcklst_Hashes.FormattingEnabled = true;
            this.chcklst_Hashes.Items.AddRange(new object[] {
            "Ed2k",
            "Sha1",
            "Md5",
            "Crc",
            "Aich",
            "Tth"});
            this.chcklst_Hashes.Location = new System.Drawing.Point(245, 28);
            this.chcklst_Hashes.Name = "chcklst_Hashes";
            this.chcklst_Hashes.Size = new System.Drawing.Size(68, 79);
            this.chcklst_Hashes.TabIndex = 2;
            this.chcklst_Hashes.Tag = "CalcHashes";
            // 
            // lbl_CalcHashes
            // 
            this.lbl_CalcHashes.AutoSize = true;
            this.lbl_CalcHashes.Location = new System.Drawing.Point(245, 12);
            this.lbl_CalcHashes.Name = "lbl_CalcHashes";
            this.lbl_CalcHashes.Size = new System.Drawing.Size(70, 13);
            this.lbl_CalcHashes.TabIndex = 3;
            this.lbl_CalcHashes.Text = "Calc Hashes:";
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = global::AVDump2GUI.Properties.Settings.Default.MakeAcreq;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::AVDump2GUI.Properties.Settings.Default, "MakeAcreq", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.checkBox1.Location = new System.Drawing.Point(12, 67);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(79, 17);
            this.checkBox1.TabIndex = 15;
            this.checkBox1.Tag = "MakeAcreq";
            this.checkBox1.Text = "Gather Info";
            this.checkBox1.UseVisualStyleBackColor = true;
            // 
            // chck_ACreq
            // 
            this.chck_ACreq.AutoSize = true;
            this.chck_ACreq.Checked = global::AVDump2GUI.Properties.Settings.Default.MakeAcreq;
            this.chck_ACreq.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chck_ACreq.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::AVDump2GUI.Properties.Settings.Default, "MakeAcreq", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chck_ACreq.Location = new System.Drawing.Point(19, 85);
            this.chck_ACreq.Name = "chck_ACreq";
            this.chck_ACreq.Size = new System.Drawing.Size(85, 17);
            this.chck_ACreq.TabIndex = 14;
            this.chck_ACreq.Tag = "MakeAcreq";
            this.chck_ACreq.Text = "Make ACreq";
            this.chck_ACreq.UseVisualStyleBackColor = true;
            // 
            // grp_Tagging
            // 
            this.grp_Tagging.Location = new System.Drawing.Point(101, 129);
            this.grp_Tagging.Name = "grp_Tagging";
            this.grp_Tagging.Size = new System.Drawing.Size(463, 117);
            this.grp_Tagging.TabIndex = 17;
            this.grp_Tagging.TabStop = false;
            this.grp_Tagging.Text = "Tagging";
            // 
            // GUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(604, 310);
            this.Controls.Add(this.tbctrl_Main);
            this.MinimumSize = new System.Drawing.Size(620, 300);
            this.Name = "GUI";
            this.Text = "AVDump2 GUI";
            this.tbctrl_Main.ResumeLayout(false);
            this.tp_Files.ResumeLayout(false);
            this.tp_Settings.ResumeLayout(false);
            this.grp_AVDump2GUISettings.ResumeLayout(false);
            this.grp_AVDump2GUISettings.PerformLayout();
            this.grp_AVDump2Settings.ResumeLayout(false);
            this.grp_AVDump2Settings.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_BlockSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Timeout)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nup_Retries)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
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
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox grp_AVDump2Settings;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.Label lbl_UserPassword;
        private System.Windows.Forms.Label lbl_Username;
        private System.Windows.Forms.TextBox txt_UserPassword;
        private System.Windows.Forms.TextBox txt_Username;
        private System.Windows.Forms.GroupBox grp_AVDump2GUISettings;
        private System.Windows.Forms.Label lbl_UseColumns;
        private System.Windows.Forms.CheckedListBox chcklst_Columns;
        private System.Windows.Forms.Panel pnl_Progress;
        private System.Windows.Forms.Label lbl_CalcHashes;
        private System.Windows.Forms.CheckedListBox chcklst_Hashes;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox chck_ACreq;
        private System.Windows.Forms.GroupBox grp_Tagging;
    }
}

