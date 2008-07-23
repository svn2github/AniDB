import subprocess
from subprocess import call, PIPE, STDOUT, list2cmdline, mswindows
import ctypes
import ctypes.wintypes
CreateProcessW = ctypes.windll.kernel32.CreateProcessW
CloseHandle = ctypes.windll.kernel32.CloseHandle

__all__ = ["Popen", "PIPE", "STDOUT", "call"]

class STARTUPINFOW(ctypes.Structure):
	_fields_ = (("cb", ctypes.wintypes.DWORD), # should init to 17*4 maybe?
		("lpReserved", ctypes.wintypes.LPWSTR),
		("lpDesktop", ctypes.wintypes.LPWSTR),
		("lpTitle", ctypes.wintypes.LPWSTR),
		("dwX", ctypes.wintypes.DWORD),
		("dwY", ctypes.wintypes.DWORD),
		("dwXSize", ctypes.wintypes.DWORD),
		("dwYSize", ctypes.wintypes.DWORD),
		("dwXCountChars", ctypes.wintypes.DWORD),
		("dwYCountChars", ctypes.wintypes.DWORD),
		("dwFillAttribute", ctypes.wintypes.DWORD),
		("dwFlags", ctypes.wintypes.DWORD),
		("wShowWindow", ctypes.wintypes.WORD),
		("cbReserved2", ctypes.wintypes.WORD),
		("lpReserved2", ctypes.wintypes.LPSTR), # actually LPBYTE
		("hStdInput", ctypes.wintypes.HANDLE),
		("hStdOutput", ctypes.wintypes.HANDLE),
		("hStdError", ctypes.wintypes.HANDLE))

class PROCESS_INFORMATION(ctypes.Structure):
	_fields_ = (("hProcess", ctypes.wintypes.HANDLE),
		("hThread", ctypes.wintypes.HANDLE),
		("dwProcessId", ctypes.wintypes.DWORD),
		("dwThreadId", ctypes.wintypes.DWORD))

if mswindows:
	class Popen(subprocess.Popen):
		def _execute_child(self, args, executable, preexec_fn, close_fds,
				cwd, env, universal_newlines,
				startupinfo, creationflags, shell,
				p2cread, p2cwrite,
				c2pread, c2pwrite,
				errread, errwrite):

			if not isinstance(args, basestring):
				args = list2cmdline(args)
			
			if not isinstance(args, unicode):
				subprocess.Popen._execute_child(self, args, executable,
					preexec_fn, close_fds, cwd, env, universal_newlines,
					startupinfo, creationflags, shell, p2cread, p2cwrite,
					c2pread, c2pwrite, errread, errwrite)
				return

			if startupinfo == None:
				startupinfo = STARTUPINFOW()
			else:
				raise NotImplementedError("Can't pass startup stuff")
			
			if not None in (p2cread, c2pwrite, errwrite):
				raise NotImplementedError("Can't pass file object bits")

			if shell:
				raise NotImplementedError("Can't pick your own shell for unicode args")
			
			margs = ctypes.create_unicode_buffer(args)
			processinfo = PROCESS_INFORMATION()

			CreateProcessW(executable, margs,
				None, None,
				1,
				creationflags,
				env, cwd,
				ctypes.byref(startupinfo), ctypes.byref(processinfo))
			
			hp, ht, pid, tid = (processinfo.hProcess, processinfo.hThread,
				processinfo.dwProcessId, processinfo.dwThreadId)
			
			# can't wrap this as a _subprocess_handle so scuppered, need sp_handle_new()
			self._handle = hp
			self.pid = pid
			CloseHandle(ht) # ht.Close()
else:
	Popen = subprocess.Popen