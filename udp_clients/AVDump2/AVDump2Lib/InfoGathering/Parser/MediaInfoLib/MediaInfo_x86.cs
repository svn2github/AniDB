using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

namespace AVDump2Lib.InfoGathering.Parser.MediaInfoLib {
	public class MediaInfo_x86 : IMediaInfo {
		//Import of DLL functions. DO NOT USE until you know what you do (MediaInfo DLL do NOT use CoTaskMemAlloc to allocate memory)  
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_New();
		[DllImport("MediaInfo_x86.dll")]
		private static extern void MediaInfo_Delete(IntPtr Handle);

		/*[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_Open(IntPtr Handle, [MarshalAs(UnmanagedType.LPWStr)] string FileName);*/

		[DllImport("MediaInfo_x86.dll", EntryPoint = "MediaInfo_Open", CharSet = CharSet.Unicode)]
		private static extern IntPtr MediaInfo_Open(IntPtr handle, IntPtr filename);


		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfoA_Open(IntPtr Handle, IntPtr FileName);
		[DllImport("MediaInfo_x86.dll")]
		private static extern void MediaInfo_Close(IntPtr Handle);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_Inform(IntPtr Handle, IntPtr Reserved);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfoA_Inform(IntPtr Handle, IntPtr Reserved);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_GetI(IntPtr Handle, IntPtr StreamKind, IntPtr StreamNumber, IntPtr Parameter, IntPtr KindOfInfo);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfoA_GetI(IntPtr Handle, IntPtr StreamKind, IntPtr StreamNumber, IntPtr Parameter, IntPtr KindOfInfo);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_Get(IntPtr Handle, IntPtr StreamKind, IntPtr StreamNumber, [MarshalAs(UnmanagedType.LPWStr)] string Parameter, IntPtr KindOfInfo, IntPtr KindOfSearch);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfoA_Get(IntPtr Handle, IntPtr StreamKind, IntPtr StreamNumber, IntPtr Parameter, IntPtr KindOfInfo, IntPtr KindOfSearch);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_Option(IntPtr Handle, [MarshalAs(UnmanagedType.LPWStr)] string Option, [MarshalAs(UnmanagedType.LPWStr)] string Value);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfoA_Option(IntPtr Handle, IntPtr Option, IntPtr Value);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_State_Get(IntPtr Handle);
		[DllImport("MediaInfo_x86.dll")]
		private static extern IntPtr MediaInfo_Count_Get(IntPtr Handle, IntPtr StreamKind, IntPtr StreamNumber);

		//MediaInfo class
		public MediaInfo_x86() {
			Handle = MediaInfo_New();
			if(Environment.OSVersion.ToString().IndexOf("Windows") == -1)
				MustUseAnsi = true;
			else
				MustUseAnsi = false;
		}
		~MediaInfo_x86() { MediaInfo_Delete(Handle); }
		public int Open(String FileName) {
			int retVal = 0;
			var t = new Thread(obj => {
				if(MustUseAnsi) {
					IntPtr FileName_Ptr = Marshal.StringToHGlobalAnsi(FileName);
					int ToReturn = (int)MediaInfoA_Open(Handle, FileName_Ptr);
					Marshal.FreeHGlobal(FileName_Ptr);
					retVal = ToReturn;
				} else {
					IntPtr stringPtr = Marshal.StringToBSTR(FileName);
					retVal = (int)MediaInfo_Open(Handle, stringPtr);
				}
			});

			t.Start();
			if(!t.Join(10000)) {
				throw new FatalException("MediaInfo Open call timed out");
			}

			return retVal;
		}
		public void Close() { MediaInfo_Close(Handle); }
		public String Inform() {
			//Environment.
			if(MustUseAnsi)
				return Marshal.PtrToStringAnsi(MediaInfoA_Inform(Handle, (IntPtr)0));
			else
				return Marshal.PtrToStringUni(MediaInfo_Inform(Handle, (IntPtr)0));
		}
		public String Get(eStreamType StreamKind, int StreamNumber, String Parameter, eInfoType KindOfInfo, eInfoType KindOfSearch) {

			if(MustUseAnsi) {
				IntPtr Parameter_Ptr = Marshal.StringToHGlobalAnsi(Parameter);
				String ToReturn = Marshal.PtrToStringAnsi(MediaInfoA_Get(Handle, (IntPtr)StreamKind, (IntPtr)StreamNumber, Parameter_Ptr, (IntPtr)KindOfInfo, (IntPtr)KindOfSearch));
				Marshal.FreeHGlobal(Parameter_Ptr);
				return ToReturn;
			} else
				return Marshal.PtrToStringUni(MediaInfo_Get(Handle, (IntPtr)StreamKind, (IntPtr)StreamNumber, Parameter, (IntPtr)KindOfInfo, (IntPtr)KindOfSearch));
		}
		public String Get(eStreamType StreamKind, int StreamNumber, int Parameter, eInfoType KindOfInfo) {
			if(MustUseAnsi)
				return Marshal.PtrToStringAnsi(MediaInfoA_GetI(Handle, (IntPtr)StreamKind, (IntPtr)StreamNumber, (IntPtr)Parameter, (IntPtr)KindOfInfo));
			else
				return Marshal.PtrToStringUni(MediaInfo_GetI(Handle, (IntPtr)StreamKind, (IntPtr)StreamNumber, (IntPtr)Parameter, (IntPtr)KindOfInfo));
		}
		public String Option(String Option, String Value) {
			if(MustUseAnsi) {
				IntPtr Option_Ptr = Marshal.StringToHGlobalAnsi(Option);
				IntPtr Value_Ptr = Marshal.StringToHGlobalAnsi(Value);
				String ToReturn = Marshal.PtrToStringAnsi(MediaInfoA_Option(Handle, Option_Ptr, Value_Ptr));
				Marshal.FreeHGlobal(Option_Ptr);
				Marshal.FreeHGlobal(Value_Ptr);
				return ToReturn;
			} else
				return Marshal.PtrToStringUni(MediaInfo_Option(Handle, Option, Value));
		}
		public int State_Get() { return (int)MediaInfo_State_Get(Handle); }
		public int Count_Get(eStreamType StreamKind, int StreamNumber) { return (int)MediaInfo_Count_Get(Handle, (IntPtr)StreamKind, (IntPtr)StreamNumber); }
		private IntPtr Handle;
		private bool MustUseAnsi;

		//Default values, if you know how to set default values in C#, say me
		public String Get(eStreamType StreamKind, int StreamNumber, String Parameter, eInfoType KindOfInfo) { return Get(StreamKind, StreamNumber, Parameter, KindOfInfo, eInfoType.Name); }
		public String Get(eStreamType StreamKind, int StreamNumber, String Parameter) { return Get(StreamKind, StreamNumber, Parameter, eInfoType.Text, eInfoType.Name); }
		public String Get(eStreamType StreamKind, int StreamNumber, int Parameter) { return Get(StreamKind, StreamNumber, Parameter, eInfoType.Text); }
		public String Option(String Option_) { return Option(Option_, ""); }
		public int Count_Get(eStreamType StreamKind) { return Count_Get(StreamKind, -1); }
	}
}
