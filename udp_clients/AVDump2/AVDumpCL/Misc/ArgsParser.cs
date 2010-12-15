using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.Xml;
using System.Xml.Linq;

namespace AVDump2CL.Misc {
	public static class ArgsParser {
		public static List<string> Parse(string cmdl) {
			Environment e = new Environment(cmdl);
			List<string> args = new List<string>();

			try {
				while(!e.EOL()) {
					if(e.Peek() == '-') {
						args.Add(ParseArg(e));
					} else {
						args.Add(ParseMedia(e, e.Peek() == '"'));
					}
					e.SkipEmpty();
				}
			} catch(Exception) { args.Clear(); }
			return args;
		}

		private static string ParseArg(Environment e) {
			e.Check(e.Peek() == '-', "Invalid beginning of Key construct");

			var arg = ParseName(e, false);
			while(!e.EOL() && e.Peek() == ':') {
				e.Advance();
				arg += ":";
				arg += ParseName(e, e.Peek() == '"').Replace(":", "|Colon|"); //Dirtyfix
			}

			return arg;
		}
		private static string ParseName(Environment e, bool isQuoted) {
			string name = "";

			if(isQuoted) e.Check("\"", "Invalid beginning of a 'Name' construct");
			while(!e.EOL() && (isQuoted || (e.Peek() != ' ' && e.Peek() != ':')) && (!isQuoted || e.Peek() != '"')) name += e.Read().ToString();
			if(isQuoted) e.Check("\"", "Invalid ending of a 'Name' construct");

			return name;
		}
		private static string ParseMedia(Environment e, bool isQuoted) {
			string name = "";

			if(isQuoted) e.Check("\"", "Invalid beginning of a 'Name' construct");
			while(!e.EOL() && (isQuoted || e.Peek() != ' ') && (!isQuoted || e.Peek() != '"')) name += e.Read().ToString();
			if(isQuoted) e.Check("\"", "Invalid ending of a 'Name' construct");

			return name;
		}

		private class Environment {
			public int CharIndex { get; set; }
			public int LineCount { get { return src.Length; } }

			string src;

			public Environment(string src) { this.src = src; }

			#region Help Methods
			public char Peek() { return src[CharIndex]; }
			public char Read() {
				char srcChar = Peek();

				Advance();
				return srcChar;
			}
			public string ReadToEnd() {
				string val = src.Substring(CharIndex);
				CharIndex = src.Length;
				return val;
			}
			public void SkipEmpty() { while(!EOL() && Peek() == ' ') { Advance(); } }
			public void Advance() { CharIndex++; }
			public int Count() { return src.Length; }
			public bool EOL() { return Count() == CharIndex; }
			public void Check(string match, string errorMsg) {
				bool isInRange = CharIndex + match.Length <= Count();
				bool isEqual = isInRange && match.Equals(src.Substring(CharIndex, match.Length));
				if(!isInRange || !isEqual) {
					throw new Exception(errorMsg);
				} else if(isInRange) {
					CharIndex += match.Length;
				} else {
					CharIndex = Count();
				}
			}
			public void Check(bool isValid, string errorMsg) { if(!isValid) throw new Exception(errorMsg); }
			public void Throw(string errorMsg) { throw new Exception(errorMsg); }
			#endregion
		}
	}

	public class AVD2ConfigFile {
		public InformationSection Information { get; private set; }
		public ExtensionsSection Extensions { get; private set; }
		public SettingsContainerSection SettingsContainer { get; private set; }

		public AVD2ConfigFile(XmlDocument xmlDoc) {
			var avDump2Node = xmlDoc.FirstChild;
			if(avDump2Node.Name != "AVDump2") throw new Exception("Invalid root node name");

			foreach(XmlNode node in avDump2Node.ChildNodes) {
				switch(node.Name) {
					case "Information": Information = new InformationSection(node); break;
					case "Extensions": Extensions = new ExtensionsSection(node); break;
					case "SettingsContainer": SettingsContainer = new SettingsContainerSection(node); break;
					default: break;
				}
			}
		}



		public class InformationSection {
			public Version TargetVersion { get; private set; }

			public InformationSection(XmlNode informationNode) {
				foreach(XmlNode node in informationNode) {
					if(node.NodeType != XmlNodeType.Element) throw new Exception("Only nods of Element type allowed");
					switch(node.Name) {
						case "TargetVersion": TargetVersion = new Version(Value(node)); break;
						default: break;
					}
				}
			}

			public override string ToString() { return "TargetVersion: " + TargetVersion; }
		}
		public class ExtensionsSection : DynReadOnlyCollection<string, ExtentionSection> {
			public ExtensionsSection(XmlNode extentionsNode)
				: base(new DynCollection<string, ExtentionSection>(ext => ext.Key)) {
				foreach(XmlNode node in extentionsNode) {
					switch(node.Name) {
						case "Extension": items.Add(new ExtentionSection(node)); break;
						default: break;
					}
				}
			}
		}
		public class ExtentionSection {
			public string Key { get; private set; }
			public string Path { get; private set; }

			public ExtentionSection(XmlNode extentionNode) {
				foreach(XmlAttribute attr in extentionNode.Attributes) {
					switch(attr.Name) {
						case "key": Key = attr.Value; break;
						case "path": Path = attr.Value; break;
						default: break;
					}
				}
			}

			public override string ToString() { return "Key: " + Key + " Path: " + Path; }
		}
		public class SettingsContainerSection : DynReadOnlyCollection<string, SettingsSection> {
			public SettingsContainerSection(XmlNode settingsContainerNode)
				: base(new DynCollection<string, SettingsSection>(s => s.Target)) {
				foreach(XmlNode node in settingsContainerNode) {
					switch(node.Name) {
						case "Settings": items.Add(new SettingsSection(node)); break;
						default: break;
					}
				}
			}
		}
		public class SettingsSection {
			private DynCollection<string, SettingNode> settingNods;
			public DynReadOnlyCollection<string, SettingNode> SettingNods { get; private set; }

			public string Target { get; private set; }

			public SettingsSection(XmlNode settingsNode) {
				foreach(XmlAttribute attr in settingsNode.Attributes) {
					switch(attr.Name) {
						case "target": Target = attr.Value; break;
						default: break;
					}
				}

				SettingNods = new DynReadOnlyCollection<string, SettingNode>(settingNods = new DynCollection<string, SettingNode>(s => s.Key));
				foreach(XmlNode node in settingsNode) settingNods.Add(new SettingNode(node));
			}

			public override string ToString() { return "Target: " + Target; }
		}

		private static string Value(XmlNode node) {
			if(node.ChildNodes.Count != 1 && node.ChildNodes[0].NodeType != XmlNodeType.Text) throw new Exception();
			return node.ChildNodes[0].Value;
		}
	}

	public class SettingNode {
		public string Key { get; private set; }
		public string Value { get; private set; }

		private DynCollection<string, SettingNode> nods;
		public DynReadOnlyCollection<string, SettingNode> Nods { get; private set; }

		private SettingNode() { Nods = new DynReadOnlyCollection<string, SettingNode>(nods = new DynCollection<string, SettingNode>(s => s.Key)); }

		public SettingNode(XmlNode settingNode)
			: this() {
			if(!char.IsLetter(settingNode.Name[0]) || !char.IsUpper(settingNode.Name[0])) throw new Exception("Keys must start with an uppercase letter");

			Key = settingNode.Name;
			foreach(XmlAttribute attr in settingNode.Attributes) {
				switch(attr.Name) {
					case "value": Value = attr.Value; break;
					case "emptyNods": foreach(var emptyNode in attr.Value.Split(',')) nods.Add(new SettingNode { Key = emptyNode }); break;
					default:
						if(!char.IsLetter(attr.Name[0]) || !char.IsUpper(attr.Name[0])) throw new Exception("Non keyword in attribute list must start with an uppercase letter");
						nods.Add(new SettingNode { Key = attr.Name, Value = attr.Value });
						break;
				}
			}

			int nodeCount = nods.Count;
			foreach(XmlNode node in settingNode) {
				switch(node.NodeType) {
					case XmlNodeType.Element: nods.Add(new SettingNode(node)); break;
					case XmlNodeType.Text:
						if(Value != null) throw new Exception("Setting the value twice is not allowed (Node: " + node.Name + ")");
						Value = node.Value;
						break;
					default: break;
				}
			}

			if(nods.Count != nodeCount && Value != null) throw new Exception("Mixed Content not allowed (Node: " + Key + ")");
		}

		public override string ToString() { return "Key: " + Key + " Value: " + Value; }
	}

	public class DynCollection<K, V> : KeyedCollection<K, V> {
		private Converter<V, K> getKey;
		public DynCollection(Converter<V, K> getKey) { this.getKey = getKey; }
		protected override K GetKeyForItem(V item) { return getKey(item); }
	}
	public class DynReadOnlyCollection<K, V> : ICollection<V> {
		protected DynCollection<K, V> items;

		public DynReadOnlyCollection(DynCollection<K, V> items) { this.items = items; }

		public void Add(V item) { throw new NotSupportedException(); }
		public void Clear() { throw new NotSupportedException(); }
		public bool Remove(V item) { throw new NotSupportedException(); }

		public bool Contains(V item) { return items.Contains(item); }
		public void CopyTo(V[] array, int arrayIndex) { items.CopyTo(array, arrayIndex); }
		public int Count { get { return items.Count; } }
		public bool IsReadOnly { get { return true; } }

		public V this[K key] { get { return items[key]; } }

		public IEnumerator<V> GetEnumerator() { return items.GetEnumerator(); }
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return items.GetEnumerator(); }
	}
}
