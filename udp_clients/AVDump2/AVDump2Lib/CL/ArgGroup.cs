using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2Lib.CL {
	public class ArgGroup {
		public string NameSpace { get; private set; }
		public string Description { get; private set; }
		public ArgStructure[] Args { get; private set; }
		public Action<string, string, object> Callback { get; private set; }

		public ArgGroup(string nameSpace, string description, Action<string, string, object> callback, params ArgStructure[] args) {
			NameSpace = nameSpace;
			Description = description;
			Callback = callback;
			Args = (ArgStructure[])args.Clone();
		}

		public override bool Equals(object obj) {
			if(obj == null || GetType() != obj.GetType()) return false;
			var argGroup = (ArgGroup)obj;
			return argGroup.NameSpace.Equals(NameSpace);
		}
		public override int GetHashCode() { return NameSpace.GetHashCode(); }
	}
}
