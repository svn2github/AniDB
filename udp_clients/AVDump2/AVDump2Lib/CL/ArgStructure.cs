using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2Lib.CL {
	public class ArgStructure {
		public string Primary { get; private set; }
		public string Secondary { get; private set; }
		public char? ShortCut { get; private set; }
		public string Description { get; private set; }
		public string Example { get; private set; }
		public Func<string, object> TransformParameter { get; private set; }

		public ArgStructure(string primary, string secondary, char? shortcut, Func<string, object> transformParameter, string example, string description) {
			Primary = primary;
			Secondary = secondary;
			ShortCut = shortcut;
			TransformParameter = transformParameter;
			Example = example;
			Description = description;
		}

		public override bool Equals(object obj) {
			if(obj == null || GetType() != obj.GetType()) return false;
			var arg = (ArgStructure)obj;
			return Primary.Equals(arg.Primary);
		}
		public override int GetHashCode() { return Primary.GetHashCode(); }

		public override string ToString() { return Primary; }
	}
}
