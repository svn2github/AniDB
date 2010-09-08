using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Data;
using AVDump2GUI.AVD2Proc;

namespace AVDump2GUI {
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window {
		public List<AVD2File> files = new List<AVD2File>();

		public MainWindow() {
			InitializeComponent();

			var files = Directory.GetFiles(@"H:\Anime", "*", SearchOption.AllDirectories);

			foreach(var file in files) {
				var avd2File = new AVD2File(new System.IO.FileInfo(file));
				avd2File.Status.Change(FileStatus.StatusType.Set | FileStatus.StatusType.Todo, AVD2Action.ACReq | AVD2Action.Consumers | AVD2Action.Information);
				this.files.Add(avd2File);
			}



			lstvw_Files.ItemsSource = this.files;
		}
	}


	public class BytesConverter : IValueConverter {
		string[] Sizes = { "B", "KB", "MB", "GB", "TB", "PG" };

		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) {
			long size = (long)value;
			int sizeType = 0;

			while(size / (1L << (10 * (sizeType + 1))) > 0) sizeType++;

			return (size / (double)(1 << (10 * sizeType))).ToString("0.00") + Sizes[sizeType];
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) { throw new NotSupportedException(); }
	}

	public class StatusConverter : IValueConverter {

		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) { return GetIconPaths((FileStatus)value); }
		private IEnumerable<string> GetIconPaths(FileStatus status) {
			yield return @"Images\" + ((status.Todo & AVD2Action.Consumers) != 0 ? "PYellow" : ((status.Done & AVD2Action.Consumers) != 0 ? "PGreen" : ((status.Failed & AVD2Action.Consumers) != 0 ? "PRed" : "PGray"))) + ".png";
			yield return @"Images\" + ((status.Todo & AVD2Action.Information) != 0 ? "FYellow" : ((status.Done & AVD2Action.Information) != 0 ? "FGreen" : ((status.Failed & AVD2Action.Information) != 0 ? "FRed" : "FGray"))) + ".png";
			yield return @"Images\" + ((status.Todo & AVD2Action.ACReq) != 0 ? "MYellow" : ((status.Done & AVD2Action.ACReq) != 0 ? "MGreen" : ((status.Failed & AVD2Action.ACReq) != 0 ? "MRed.png" : "MGray"))) + ".png";
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture) { throw new NotSupportedException(); }
	}
}
