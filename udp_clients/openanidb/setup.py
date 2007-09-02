from distutils.core import setup
import py2exe

opts = {
}

setup(
	options = opts,
	name = "OADB",
	description = "OpenAniDB",
	version = "0.4",
	windows = [
		{
			"script": "openanidb.py",
			"icon_resources": [(0, "zero.ico")]
		}
	],
	data_files = [(".", ["COPYING", "CHANGELOG", "README", "zero.ico", "zero.xpm"])]
)
