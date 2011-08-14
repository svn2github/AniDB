using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace AVDump2Lib.CL {
	public class CLManagement {
		private List<ArgGroup> argGroups;
		private Action<string> unnamedArgHandler;

		public CLManagement() { argGroups = new List<ArgGroup>(); }


		public void RegisterArgGroup(ArgGroup argGroup) {
			if(argGroup == null) throw new ArgumentNullException("argGroup");
			if(argGroups.Any(ldArgGroup => ldArgGroup.NameSpace.Equals(argGroup.NameSpace))) throw new Exception("Owner already registered");
			argGroups.Add(argGroup);
		}

		public void SetUnnamedParamHandler(Action<string> unnamedArgHandler) { this.unnamedArgHandler = unnamedArgHandler; }


		public bool ParseArgs(string[] args) {

			if(args.Length > 0 && args[0].ToLower().Equals("--Help".ToLower())) {
				if(args.Length == 2) PrintTopic(args[1], true); else PrintHelp(true);
				return false;
			}

			object transformedParam;
			string nameSpace = null, name, param;
			for(int i = 0;i < args.Length;i++) {
				//if(string.IsNullOrEmpty(args[i].Trim())) continue;

				var match = Regex.Match(args[i], @"^--?(?:(?<NameSpace>[a-zA-Z0-9]+)\.)?(?<Arg>[a-zA-Z0-9][a-zA-Z0-9\-]*)(?:=(?<Param>.*))?$");
				nameSpace = match.Groups["NameSpace"].Success ? match.Groups["NameSpace"].Value : null;
				param = match.Groups["Param"].Success ? match.Groups["Param"].Value : null;
				name = match.Groups["Arg"].Value;

				if(args[i][0] == '-' && name.Equals(string.Empty)) throw new Exception("Invalid argument structure");
				if(args[i][0] == '-' && args[i][1] != '-') {
					if(name.Length > 1) {
						if(param != null) throw new Exception("Multiple one letter arguments may not have parameters");
						ParseArgs(name.Select(ldSwitch => "--" + (nameSpace != null ? nameSpace + "." : "") + ldSwitch).ToArray());
					} else {
						ParseArgs(new string[] { "--" + (nameSpace != null ? nameSpace + "." : "") + name + (param != null ? "=" + param : "") });
					}

				} else if(args[i][0] == '-' && args[i][1] == '-') {
					var q = from g in argGroups
							where nameSpace == null || nameSpace.ToLower().Equals(g.NameSpace.ToLower())
							from a in g.Args
							let n = name.ToLower()
							where n.Equals(a.Primary.ToLower()) || n.Equals(a.Secondary.ToLower()) || (a.ShortCut.HasValue && name.Equals(a.ShortCut.Value.ToString()))
							select new { Group = g, Arg = a };

					switch(q.Count()) {
						case 0: throw new Exception("Argument (" + (!string.IsNullOrEmpty(nameSpace) ? nameSpace + "." : "") + name + ") is not registered");
						case 1: break;
						default: throw new Exception("Argument reference is ambiguous: " + string.Join(", ", q.Select(ldQuery => ldQuery.Group.NameSpace + "." + (args[i][1] == '-' ? ldQuery.Arg.Primary : ldQuery.Arg.Secondary)).ToArray()));
					}
					var entry = q.First();

					transformedParam = null;
					if((param == null) ^ (entry.Arg.TransformParameter == null)) {
						throw new Exception("Parameter count doesn't match for " + (!string.IsNullOrEmpty(nameSpace) ? nameSpace + "." : "") + name + " (Out of bounds)");
					} else if(entry.Arg.TransformParameter != null) {
						try {
							transformedParam = entry.Arg.TransformParameter(param);
						} catch(Exception ex) {
							throw new Exception("Couldn't transform parameter for " + (!string.IsNullOrEmpty(nameSpace) ? nameSpace + "." : "") + name + " (Check for parameter validity)", ex);
						}
					}

					try {
						entry.Group.Callback(entry.Group.NameSpace, entry.Arg.Primary, transformedParam);
					} catch(Exception ex) {
						throw new Exception("Arg handler (" + entry.Group.NameSpace + "." + entry.Arg.Primary + ") threw an error", ex);
					}

				} else if(unnamedArgHandler != null) unnamedArgHandler(args[i]);

			}
			return true;
		}


		public void PrintHelp(bool detailed) {
			foreach(var argGroup in argGroups) PrintTopic(argGroup.NameSpace, detailed);
			if(!detailed) {
				Console.WriteLine("Use --Help OR --Help <NameSpace> for more detailed info");
				Console.WriteLine();
			}
		}

		public void PrintTopic(string topic, bool detailed) {
			var argGroup = argGroups.SingleOrDefault(ldArgGroup => ldArgGroup.NameSpace.ToLower().Equals(topic.ToLower()));
			if(argGroup == null) {
				Console.WriteLine("There is no such topic");
				Console.WriteLine();
				return;
			}

			Console.ForegroundColor = ConsoleColor.DarkGreen;
			Console.WriteLine("NameSpace: " + argGroup.NameSpace + (!string.IsNullOrEmpty(argGroup.Description) ? " | " + argGroup.Description : ""));

			string primary, secondary, shortCut, example;
			int primaryPad = argGroup.Args.Max(ldArg => ldArg.Primary.Length + 3);
			int secondaryPad = argGroup.Args.Max(ldArg => !string.IsNullOrEmpty(ldArg.Secondary) ? ldArg.Secondary.Length + 3 : 0);
			int shortCutPad = argGroup.Args.Max(ldArg => ldArg.ShortCut.HasValue ? 3 : 0);
			foreach(var arg in argGroup.Args) {
				Console.ForegroundColor = ConsoleColor.White;

				primary = ("--" + arg.Primary).PadRight(primaryPad);
				secondary = (!string.IsNullOrEmpty(arg.Secondary) ? "--" + arg.Secondary : "").PadRight(secondaryPad);
				shortCut = (arg.ShortCut.HasValue ? "-" + arg.ShortCut : "").PadRight(shortCutPad);
				example = (!string.IsNullOrEmpty(arg.Example) ? "| " + arg.Example : "");
				Console.WriteLine(primary + secondary + shortCut + example);
				Console.ResetColor();
				if(detailed && !string.IsNullOrEmpty(arg.Description)) {
					if(!string.IsNullOrEmpty(arg.Description)) Console.WriteLine(arg.Description);
					Console.WriteLine();
				}
			}
			Console.WriteLine();
		}
	}
}
