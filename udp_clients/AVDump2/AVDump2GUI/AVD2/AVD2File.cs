using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AVDump2GUI.AVD2Proc {
	public class AVD2File {
		public FileStatus Status { get; private set; }
		public FileInfo File { get; private set; }

		public AVD2File(FileInfo fileInfo) { this.File = fileInfo; Status = new FileStatus(); }
	}

	public interface IFileStatus { AVD2Action Todo { get; } AVD2Action Done { get; } AVD2Action Failed { get; } }
	public class FileStatus : IFileStatus {
		public delegate void ChangeStatus(StatusType type, AVD2Action action);

		private AVD2Action todo; public AVD2Action Todo { get { return todo; } }
		private AVD2Action done; public AVD2Action Done { get { return done; } }
		private AVD2Action failed; public AVD2Action Failed { get { return failed; } }

		//public FileStatus(out ChangeStatus changeStatus) { changeStatus = new ChangeStatus(Change); }

		public ReadOnlyFileStatus AsReadOnly() { return new ReadOnlyFileStatus(this); }

		public void Change(StatusType type, AVD2Action action) {
			switch(type & StatusType.ModifierMask) {
				case StatusType.Set:
					switch(type & StatusType.TypeMask) {
						case StatusType.Todo: Set(action, ref todo, ref done, ref failed); break;
						case StatusType.Done: Set(action, ref done, ref todo, ref failed); break;
						case StatusType.Failed: Set(action, ref failed, ref todo, ref done); break;
						default: throw new ArgumentException("Invalid type value (TypeMask)", "type");
					}
					break;
				case StatusType.UnSet:
					todo &= ~action; done &= ~action; failed &= ~action;
					break;
				default: throw new ArgumentException("Invalid type value (ModifierMask)", "type");
			}
		}
		private void Set(AVD2Action actions, ref AVD2Action toSet, ref AVD2Action toUnsetA, ref AVD2Action toUnsetB) { toSet |= actions; toUnsetA &= ~actions; toUnsetB &= ~actions; }

		public class ReadOnlyFileStatus : IFileStatus {
			public ReadOnlyFileStatus(FileStatus status) { this.status = status; }

			private FileStatus status;
			public AVD2Action Todo { get { return status.todo; } }
			public AVD2Action Done { get { return status.done; } }
			public AVD2Action Failed { get { return status.failed; } }
		}

		[Flags]
		public enum StatusType { Set = 8, UnSet = 16, ModifierMask = Set | UnSet, Todo = 1, Done = 2, Failed = 4, TypeMask = Todo | Done | Failed }
	}
	[Flags]
	public enum AVD2Action { Consumers = 1, Information = 2, ACReq = 4 }
}
