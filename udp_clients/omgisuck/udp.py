import Queue, socket, time, threading, random

#connection data
server = "api.anidb.info"
port = 9000
local_port = 13245
apiversion = "3"
natted_connection = True
retries = 3

#client data
clientversion = "1"
clientname = "omgisuck"

#data definition (packages received from anidb)
datadef = {
	"anime":
	["aid", "year", "related_aid", "related_type", "category", "romaji", "kanji", "english", "eps", "rating", "vote_count", "tmprating", "tmpvote_count", "review_rating", "review_count" ],
	"episodes":
	["eid", "aid", "length", "erating", "evotes", "epno", "eenglish", "eromaji", "ekanji"],
	"files":
	["fid", "aid", "eid", "gid", "state", "size", "ed2k", "filename"],
	"groups":
	["gid", "grating", "gvotes", "acount", "fcount", "gname", "gshort", "irc", "ircserver", "url"],
}

class udpthread(threading.Thread):
	'''Thread that runs a UDP request queue. Handles timeouts, errors,
	MTU, and flood control automatically. Not bad for datagrams, huh?'''

	def __init__(self):
		threading.Thread.__init__(self)
		# In...
		self.i = Queue.Queue(5)
		# And out...
		self.o = Queue.Queue(5)
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		try:
			self.sock.bind(('0.0.0.0', local_port))
		except Exception:
			self.sock.bind(('0.0.0.0', random.randint(10000,20000)))
		self.sock.settimeout(20)

	def run(self):
		last_cmd_sent = time.time()
		while True:
			if not self.i.empty():
				# This thread's in is the system's out.
				outbound = self.i.get(False)
				print "UDP: Sending", outbound
				self.sock.sendto(outbound, (server, port))
				try:
					inbound = self.sock.recv(1400)
					print "UDP: Received", inbound
					if inbound[0:3]  ==  "505":
						print "Error: Server returned 'Illegal input recieved.'\nThe program's state may not be consistent. Please restart\nthe program, and email the following to the developers:\n\nUDP: Sent " + outbound + "\nUDP: Recieved " + inbound
					elif inbound[0:3]  ==  "555":
						print "Error: You have been temporarily banned. Common reasons include:\n - You have flooded the server with too many requests\n - You have logged in too many times from this IP without logging out\nBans may last anywhere from 5 minutes to 30 minutes. Please be patient\n and try again later. Thanks!"
					self.o.put(inbound.decode("utf8"))
					last_cmd_sent = time.time()
				except socket.timeout:
					print "UDP: Timeout!"
					print "Error: Database timed out. You may be flooding,\nor the server may be down for maintenance.\nPlease wait a second and then try again."
					self.o.put(None)
			if time.time() >= last_cmd_sent+300:
				#we haven't sent anything to the server in the past 5min. let's send a ping to keep connectivity
				self.i.put("PING nat=1")
				last_cmd_sent = time.time()
			time.sleep(2)

class udpoperations():

	def __init__(self):
		self.session = None
		self.conn = udpthread()
		self.conn.setDaemon(True)
		self.conn.start()

	def nat(self):
		if natted_connection:
			return "&nat=1"
		else:
			return ""

	def ping(self):
		'''Ping the database. We do not need a login for this.
		Returns True on success, otherwise False.'''
		self.conn.i.put("PING nat=1")
		data = self.conn.o.get()
		return self.codeparser(data)

	def login(self, user, passwd):
		'''Start a self.session. Returns self.session id on success, None on failure.'''
		command = "AUTH user=" + user.lower() + "&pass=" + passwd + "&protover=" + apiversion + "&client=" + clientname + "&clientver=" + clientversion + self.nat()
		self.conn.i.put(command)
		data = self.conn.o.get()
		if self.codeparser(data) is True:
			(trash, data) = data.split(" ", 1)
			(self.session, trash) = data.split(" ", 1)
			return True
		else:
			return False

	def logout(self):
		'''End a self.session.'''
		if self.session  ==  None:
			return True
		command = "LOGOUT s=" + self.session + self.nat()
		self.conn.i.put(command)
		data = self.conn.o.get()
		if self.codeparser(data) is True:
			self.session = None
			return True
		else:
			return False

	def send_command(self, type, command):
		if self.session is None:
			return None
		data = None
		for i in range(retries):
			self.conn.i.put(command + self.nat())
			data = self.conn.o.get()
			if data is not None:
				data = self.unamp(data)
				if self.codeparser(data) is True:
					(trash1, trash2, datastr) = data.split(" ",2)
					return self.convert_data(type, datastr)
				else:
					# No such anime.
					return None
			else:
				#the server gave us a timeout. let's try this again after waiting for a bit
				i = i+1
				sleep(2)
		return data

	def get_anime(self, arg):
		'''Retrieves data for an anime.'''
		if arg is None:
			return False
		elif arg.isdigit():
			command = "ANIME aid=" + arg + "&s=" + self.session + "&amask=AEE080FC00"
		else:
			command = "ANIME aname=" + arg + "&s=" + self.session + "&amask=AEE080FC00"
		return self.send_command("anime", command)

	#####################################################
	#													#
	#				NOT YET IMPLEMENTED					#
	#													#
	#####################################################
	"""
	def get_episode(self, eid=0, aid=()):
		'''Gets an episode. Supports the caching mechanism.'''
		if eid  ==  0 and aid  ==  ():
			return False
		# Since ID is more accurate than name, it gets done first...
		if eid != 0:
			command = "EPISODE eid=" + str(eid) + "&s=" + self.session
		else:
			# Sanest way to pass epno
			command = "EPISODE aid=" + str(aid[0]) + "&epno=" + str(aid[1]).lstrip("0") + "&s=" + self.session
		self.conn.i.put(command)
		data = self.conn.o.get()
		data = self.unamp(data)
		if self.codeparser(data) is True:
			(trash1, trash2, datastr) = data.split(" ",2)
			return self.convert_data("episode", datastr)
		else:
			# No such anime.
			return None

	def get_file(self, fid=0, file=()):
		'''Issues a FILE command.'''
		if fid  ==  0 and file  ==  ():
			# WTF?
			return False
		elif self.session  ==  None:
			# If self.session is empty, then searching the cache is the limit...
			return None
		if fid != 0:
			command = "FILE fid=" + str(fid) + "&s=" + self.session
		else:
			command = "FILE size=" + str(file[1]) + "&ed2k=" + file[0].lower() + "&s=" + self.session
		self.conn.i.put(command)
		data = self.conn.o.get()
		data = self.unamp(data)
		if self.codeparser(data) is True:
			(trash1, trash2, datastr) = data.split(" ",2)
			return self.convert_data("file", datastr)
		else:
			# No such anime.
			return None

	def get_group(self, gid=0):
		'''Issues a FILE command.'''
		if gid  ==  0:
			return False
		elif self.session  ==  None:
			return None
		elif gid != 0:
			command = "GROUP gid=" + str(gid) + "&s=" + self.session
		self.conn.i.put(command)
		data = self.conn.o.get()
		data = self.unamp(data)
		if self.codeparser(data) is True:
			(trash1, trash2, datastr) = data.split(" ",2)
			return self.convert_data("group", datastr)
		else:
			# No such anime.
			return None
	"""

	def set_encoding(self):
		'''Sets self.session encoding. There is no reason to change it away from UTF8 Unicode, so don\'t touch!'''
		command = "ENCODING name=UTF8&s=" + self.session + self.nat()
		self.conn.i.put(command)
		data = self.conn.o.get()
		if self.codeparser(data) is True:
			return True
		else:
			return False

	def unamp(self,str):
		'''Unescapes a string.'''
		return str.replace("\n"," ").strip()

	def convert_data(self, datatype, datastr):
		i=0
		dataset = {}
		data = datastr.split("|")
		for elem in data:
			try:
				dataset[datadef[datatype][i]] = elem
			except:
				print "ERROR AT ELEMENT " + str(i) + " "
				try:
					print elem
				except:
					pass
				pass
			i = i + 1
		return dataset

	def codeparser(self, tmp):
		try:
			(code, data) = tmp.split(" ", 1)
			#print data

			"""all return codes are set to False to fail by default. enable them as you need to"""
			if int(code)==200:
				"""LOGIN_ACCEPTED"""
				return True
			elif int(code)==201:
				"""LOGIN_ACCEPTED_NEW_VER"""
				return True
			elif int(code)==203:
				"""LOGGED_OUT"""
				return True
			elif int(code)==205:
				"""RESOURCE"""
				return False
			elif int(code)==206:
				"""STATS"""
				return False
			elif int(code)==207:
				"""TOP"""
				return False
			elif int(code)==208:
				"""UPTIME"""
				return False
			elif int(code)==209:
				"""ENCRYPTION_ENABLED"""
				return False
			elif int(code)==210:
				"""MYLIST_ENTRY_ADDED"""
				return False
			elif int(code)==211:
				"""MYLIST_ENTRY_DELETED"""
				return False
			elif int(code)==214:
				"""ADDED_FILE"""
				return False
			elif int(code)==215:
				"""ADDED_STREAM"""
				return False
			elif int(code)==219:
				"""ENCODING_CHANGED"""
				return True
			elif int(code)==220:
				"""FILE"""
				return True
			elif int(code)==221:
				"""MYLIST"""
				return False
			elif int(code)==222:
				"""MYLIST_STATS"""
				return False
			elif int(code)==230:
				"""ANIME"""
				return True
			elif int(code)==231:
				"""ANIME_BEST_MATCH"""
				return False
			elif int(code)==232:
				"""RANDOMANIME"""
				return False
			elif int(code)==233:
				"""ANIME_DESCRIPTION"""
				return False
			elif int(code)==240:
				"""EPISODE"""
				return True
			elif int(code)==245:
				"""PRODUCER"""
				return False
			elif int(code)==250:
				"""GROUP"""
				return True
			elif int(code)==253:
				"""BUDDY_LIST"""
				return False
			elif int(code)==254:
				"""BUDDY_STATE"""
				return False
			elif int(code)==255:
				"""BUDDY_ADDED"""
				return False
			elif int(code)==256:
				"""BUDDY_DELETED"""
				return False
			elif int(code)==257:
				"""BUDDY_ACCEPTED"""
				return False
			elif int(code)==258:
				"""BUDDY_DENIED"""
				return False
			elif int(code)==260:
				"""VOTED"""
				return False
			elif int(code)==261:
				"""VOTE_FOUND"""
				return False
			elif int(code)==262:
				"""VOTE_UPDATED"""
				return False
			elif int(code)==263:
				"""VOTE_REVOKED"""
				return False
			elif int(code)==270:
				"""NOTIFICATION_ENABLED"""
				return False
			elif int(code)==271:
				"""NOTIFICATION_NOTIFY"""
				return False
			elif int(code)==272:
				"""NOTIFICATION_MESSAGE"""
				return False
			elif int(code)==273:
				"""NOTIFICATION_BUDDY"""
				return False
			elif int(code)==274:
				"""NOTIFICATION_SHUTDOWN"""
				return False
			elif int(code)==280:
				"""PUSHACK_CONFIRMED"""
				return False
			elif int(code)==281:
				"""NOTIFYACK_SUCCESSFUL_M"""
				return False
			elif int(code)==282:
				"""NOTIFYACK_SUCCESSFUL_N"""
				return False
			elif int(code)==290:
				"""NOTIFICATION"""
				return False
			elif int(code)==291:
				"""NOTIFYLIST"""
				return False
			elif int(code)==292:
				"""NOTIFYGET_MESSAGE"""
				return False
			elif int(code)==293:
				"""NOTIFYGET_NOTIFY"""
				return False
			elif int(code)==294:
				"""SENDMSG_SUCCESSFUL"""
				return False
			elif int(code)==295:
				"""USER"""
				return False
			elif int(code)==300:
				"""PONG"""
				return True
			elif int(code)==301:
				"""AUTHPONG"""
				return True
			elif int(code)==305:
				"""NO_SUCH_RESOURCE"""
				return False
			elif int(code)==309:
				"""API_PASSWORD_NOT_DEFINED"""
				return False
			elif int(code)==310:
				"""FILE_ALREADY_IN_MYLIST"""
				return False
			elif int(code)==311:
				"""MYLIST_ENTRY_EDITED"""
				return False
			elif int(code)==312:
				"""MULTIPLE_MYLIST_ENTRIES"""
				return False
			elif int(code)==314:
				"""SIZE_HASH_EXISTS"""
				return False
			elif int(code)==315:
				"""INVALID_DATA"""
				return False
			elif int(code)==316:
				"""STREAMNOID_USED"""
				return False
			elif int(code)==320:
				"""NO_SUCH_FILE"""
				return False
			elif int(code)==321:
				"""NO_SUCH_ENTRY"""
				return False
			elif int(code)==322:
				"""MULTIPLE_FILES_FOUND"""
				return False
			elif int(code)==330:
				"""NO_SUCH_ANIME"""
				return False
			elif int(code)==333:
				"""NO_SUCH_ANIME_DESCRIPTION"""
				return False
			elif int(code)==340:
				"""NO_SUCH_EPISODE"""
				return False
			elif int(code)==345:
				"""NO_SUCH_PRODUCER"""
				return False
			elif int(code)==350:
				"""NO_SUCH_GROUP"""
				return False
			elif int(code)==355:
				"""BUDDY_ALREADY_ADDED"""
				return False
			elif int(code)==356:
				"""NO_SUCH_BUDDY"""
				return False
			elif int(code)==357:
				"""BUDDY_ALREADY_ACCEPTED"""
				return False
			elif int(code)==358:
				"""BUDDY_ALREADY_DENIED"""
				return False
			elif int(code)==360:
				"""NO_SUCH_VOTE"""
				return False
			elif int(code)==361:
				"""INVALID_VOTE_TYPE"""
				return False
			elif int(code)==362:
				"""INVALID_VOTE_VALUE"""
				return False
			elif int(code)==363:
				"""PERMVOTE_NOT_ALLOWED"""
				return False
			elif int(code)==364:
				"""ALREADY_PERMVOTED"""
				return False
			elif int(code)==370:
				"""NOTIFICATION_DISABLED"""
				return False
			elif int(code)==380:
				"""NO_SUCH_PACKET_PENDING"""
				return False
			elif int(code)==381:
				"""NO_SUCH_ENTRY_M"""
				return False
			elif int(code)==382:
				"""NO_SUCH_ENTRY_N"""
				return False
			elif int(code)==392:
				"""NO_SUCH_MESSAGE"""
				return False
			elif int(code)==393:
				"""NO_SUCH_NOTIFY"""
				return False
			elif int(code)==394:
				"""NO_SUCH_USER"""
				return False
			elif int(code)==403:
				"""NOT_LOGGED_IN"""
				return True
			elif int(code)==410:
				"""NO_SUCH_MYLIST_FILE"""
				return False
			elif int(code)==411:
				"""NO_SUCH_MYLIST_ENTRY"""
				return False
			elif int(code)==500:
				"""LOGIN_FAILED"""
				return False
			elif int(code)==501:
				"""LOGIN_FIRST"""
				return False
			elif int(code)==502:
				"""ACCESS_DENIED"""
				return False
			elif int(code)==503:
				"""CLIENT_VERSION_OUTDATED"""
				return False
			elif int(code)==504:
				"""CLIENT_BANNED"""
				return False
			elif int(code)==505:
				"""ILLEGAL_INPUT_OR_ACCESS_DENIED"""
				return False
			elif int(code)==506:
				"""INVALID_SESSION"""
				return False
			elif int(code)==509:
				"""NO_SUCH_ENCRYPTION_TYPE"""
				return False
			elif int(code)==519:
				"""ENCODING_NOT_SUPPORTED"""
				return False
			elif int(code)==555:
				"""BANNED"""
				return False
			elif int(code)==598:
				"""UNKNOWN_COMMAND"""
				return False
			elif int(code)==600:
				"""INTERNAL_SERVER_ERROR"""
				return False
			elif int(code)==601:
				"""ANIDB_OUT_OF_SERVICE"""
				return False
			elif int(code)==602:
				"""SERVER_BUSY"""
				return False
			elif int(code)==666:
				"""API_VIOLATION"""
				return False
			else:
				"""some unknown return code"""
				return False
		except:
			print "ERROR WHILE CODEPARSING THIS STRING: " + tmp
			return False