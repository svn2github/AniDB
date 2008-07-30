import subprocessw
from subprocess import mswindows, PIPE
from threading import Thread, RLock, Event
from Queue import Queue
from types import FunctionType

__all__ = ['AvdumpThread', 'AvdumpHandler']

# this is ugly, broken and appallingly magic, the result of much hacking to get it working at all
class namedproperty(property):
	def __init__(self, name, *args, **kw):
		largs = []
		lkw = {}
		for f in args:
			g = f.func_globals.copy()
			g['name'] = name
			largs.append(FunctionType(f.func_code, g, f.func_name, f.func_defaults, f.func_closure))
		for i, f in kw.items():
			g = f.func_globals.copy()
			g['name'] = name
			kw[i] = FunctionType(f.func_code, g, f.func_name, f.func_defaults, f.func_closure)
		property.__init__(self, *largs, **lkw)

class AvdumpHandler(object):
	def avdump_idle(self):
		raise NotImplementedError()
	def avdump_ed2k(self, str):
		raise NotImplementedError()

class AvdumpThread(Thread):
	def _getlattr(self):
		self._datalock.acquire()
		try:
			val = getattr(self, '_locked_' + name)
		finally:
			self._datalock.release()
		return val

	def _setlattr(self, val):
		self._datalock.acquire()
		try:
			setattr(self, '_locked_' + name, val)
		finally:
			self._datalock.release()

	avdumppath = namedproperty('avdumppath', _getlattr, _setlattr)
	exportpath = namedproperty('exportpath', _getlattr, _setlattr)
	username = namedproperty('username', _getlattr, _setlattr)
	apikey = namedproperty('apikey', _getlattr, _setlattr)
	handler = namedproperty('handler', _getlattr, _setlattr)

	def __init__(self, handler):
		Thread.__init__(self)
		self._datalock = RLock()
		self._wakeupevent = Event()
		self._wakeupevent.clear()
		self._queue = []
		self.handler = handler
		self._count = 0

	def queue(self, *paths):
		self._datalock.acquire()
		try:
			self._queue.extend(paths)
		finally:
			self._datalock.release()
		self._wakeupevent.set()

	def run(self):
		while True:
			self._wakeupevent.wait()
			self._wakeupevent.clear()
			while True:
				self._datalock.acquire()
				try:
					paths = tuple(self._queue[:])
				finally:
					self._datalock.release()
				if not paths:
					break
				args = u'"%s" -oev -exp:"%s" -ac:%s:%s' % (self.avdumppath, self.exportpath, self.username, self.apikey)
				args += (u' %s' * len(paths)) % paths
				self._count = len(paths)
				process = subprocessw.Popen(args, shell=not mswindows, stdout=PIPE)
				self._processoutput(process.stdout)
				# if self._count != 0: self.handler.error
			self.handler.avdump_idle()

	def _processoutput(self, stdout):
		while True:
			line = stdout.readline()
			if not line:
				break
			if line.startswith('File number') and self._count > 0:
				self._datalock.acquire()
				try:
					del self._queue[0]
				finally:
					self._datalock.release()
				self._count -= 1
			if line.startswith('  ed2k:'):
				self.handler.avdump_ed2k(line[2:])
