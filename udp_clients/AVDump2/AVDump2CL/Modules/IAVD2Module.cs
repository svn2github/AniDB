using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2CL.Modules {
	public interface IAVD2HashAlgorithm {

	}
	public interface IAVD2BlockConsumer {

	}

	public interface IAVD2Module {
		Version ModuleVersion { get; }
		bool CompatibleVersion(Version version, VersionType versionType);

	}
	public enum VersionType { Settings, AVDump2 }

	public interface IAVD2PostProcessing {

	}

}
