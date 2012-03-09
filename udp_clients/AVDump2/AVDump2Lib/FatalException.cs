using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2Lib {
	[Serializable]
	public class FatalException : Exception {
		public FatalException() { }
		public FatalException(string message) : base(message) { }
		public FatalException(string message, Exception inner) : base(message, inner) { }
		protected FatalException(
		  System.Runtime.Serialization.SerializationInfo info,
		  System.Runtime.Serialization.StreamingContext context)
			: base(info, context) { }
	}
}
