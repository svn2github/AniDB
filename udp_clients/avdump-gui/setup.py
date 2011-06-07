from distutils.core import setup
import py2exe

setup(
    windows=['avdump2gui.pyw'],
    options = {
        'py2exe': {
            'packages'     : ['sip'],
            'optimize'     : 2,
            'bundle_files' : 1,
            'dll_excludes' : ['w9xpopen.exe', 'mswsock.dll', 'powrprof.dll']
        }
    },
    zipfile = None
)
