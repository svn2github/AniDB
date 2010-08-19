using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2CL.Misc {
	public static class Extensions {
		public static string ToFormatedString(this TimeSpan ts) { return ((int)ts.TotalHours).ToString("00") + ":" + ts.Minutes.ToString("00") + ":" + ts.Seconds.ToString("00"); }
	}
}
