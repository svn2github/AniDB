from esky.bdist_esky import Executable
from distutils.core import setup

setup(
    name = "avdump-gui",
    version = "2.1.7",
    scripts = [
        Executable(
            "avdump2gui.pyw",
            icon='anidb.ico',
            gui_only=True
        )
    ],
    options = {
        "bdist_esky": {
            "freezer_module"   : "py2exe",
            "freezer_options"  : {
                "optimize"     : 2,
                "bundle_files" : 3,
                "compressed"   : True,
                "packages"     : ['sip'],
                "dll_excludes" : [
                    'w9xpopen.exe',
                    'mswsock.dll',
                    'powrprof.dll'
                ]
            }
        }
    },
    data_files = [
        'msvcp90.dll',
        'MediaInfo_x86.dll',
        'MediaInfo_x64.dll',
        'Ionic.Zip.Reduced.dll',
        'CSEBMLLib.dll',
        'AVDump2Lib.dll.config',
        'AVDump2Lib.dll',
        'AVDump2CL.exe'
    ]
)
