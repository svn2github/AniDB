using System;
using System.Windows.Forms;
using Kucing.AniDB.MyList;

namespace Program
{
    public partial class MyForm : Form
    {

        TreeView tvAnime;
        MyList myList;

        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MyForm());
        }

        public MyForm()
        {
            this.tvAnime = new TreeView();
            this.SuspendLayout();
            this.tvAnime.Dock = DockStyle.Fill;
            this.Controls.Add(this.tvAnime);
            this.DoubleBuffered = true;
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Load += new System.EventHandler(this.MyForm_Load);
            this.ResumeLayout(false);
        }

        private void MyForm_Load(object sender, System.EventArgs e)
        {
            var t0 = DateTime.Now;
            try { myList = MyList.Load(@"mylist.xml"); }
            catch (Exception ex) { MessageBox.Show(ex.Message + "\n" + ex.InnerException.Message); Application.Exit(); return; }
            var t1 = DateTime.Now;
            foreach (var anime in myList.Animes)
            {
                var animeNode = tvAnime.Nodes.Add(anime.ToString());
                foreach (var episode in anime.Episodes)
                {
                    var episodeNode = animeNode.Nodes.Add(episode.ToString());
                    foreach (var file in episode.Files)
                        episodeNode.Nodes.Add(file.ToString());
                }
            }
            var t2 = DateTime.Now;
            Text = myList.User.Name + " | Xml:" + (t1 - t0) + "s | Form:" + (t2 - t1) + "s";
        }
    }
}
