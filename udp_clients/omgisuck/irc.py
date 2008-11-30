from ircbot import SingleServerIRCBot
from irclib import nm_to_n, nm_to_h, irc_lower, ip_numstr_to_quad, ip_quad_to_numstr
import udp

udp_user = "testidiot"
udp_passwd = "12345678"
irc_channel = "#anidb-test"
irc_server = "irc.synirc.net"
irc_port = 6667
irc_nickname = "CC[AR]"

class ircbot(SingleServerIRCBot):

	def __init__(self, channel, nickname, server, port):
		SingleServerIRCBot.__init__(self, [(server, port)], nickname, nickname)
		self.channel = channel

	def connect_to_anidb(self):
		self.udp = udp.udpoperations()
		if self.udp.session is None:
			if self.udp.ping():
				if self.udp.login(udp_user, udp_passwd):
					if not self.udp.set_encoding():
						print "Warning: Couldn't negotiate Unicode encoding;\nJapanese characters may not appear correctly."
					return True
			else:
				print "AniDB not responding. Try again in a minute."
				return False

	#####################################################
	#													#
	#			IRC FUNCTIONS AND EVENTS				#
	#													#
	#####################################################

	def on_welcome(self, connection, event):
		connection.join(self.channel)

	def on_nicknameinuse(self, connection, event):
		connection.nick(connection.get_nickname() + "_")

	def on_privmsg(self, connection, event):
		self.do_command(event, event.arguments()[0])

	def on_pubmsg(self, connection, event):
		self.do_command(event, event.arguments()[0])

	def do_command(self, event, cmd):
		nick = nm_to_n(event.source())
		connection = self.connection

		if event.eventtype() == "privmsg":
			target = nick
		else:
			target = self.channel

		try:
			(cmd, arg) = cmd.split(" ",1)
		except:
			arg = ""
			pass

		if cmd == "!disconnect":
			if event.eventtype() == "privmsg":
				self.disconnect()
		elif cmd == "!die":
			if event.eventtype() == "privmsg":
				self.udp.logout()
				self.die()
		elif cmd == "!reconnect":
			if event.eventtype() == "privmsg":
				self.connect_to_anidb
		elif cmd == "!anime":
			if arg != "":
				connection.privmsg(target, self.get_anime_output(self.udp.get_anime(arg), arg).encode("utf8"))
			else:
				connection.privmsg(target, "provide an argument you tard")
		else:
			connection.privmsg(target, "Not understood: " + cmd)

	#####################################################
	#													#
	#				CONVENIENCE FUNCTIONS				#
	#													#
	#####################################################

	def convert_vote(self, value):
		return float(value)/100

	def check_year(self, year):
		tmp = year.split("-")
		if tmp[0]==tmp[1]:
			return tmp[0]
		else:
			return year

	def check_categories(self, categories):
		try:
			tmp = categories.split(",")
			while len(tmp)>5:
				del tmp[len(tmp)-1]
			categories = ", ".join(tmp)
		except:
			#only 1 element hence can't split
			pass
		return categories

	def set_anime_relations(self, data):
		sequel = 1
		prequel = 2

		#print data
		try:
			tmp = data['related_aid']
			try:
				rel_aids = data['related_aid'].split("'")
				rel_types = data['related_type'].split("'")
				relations = {}

				for i in range(len(rel_aids)):
					relations[rel_aids[i]] = rel_types[i]
			except:
				#only 1 relation
				relations[data['related_aid']] = data['related_type']

			print relations

			prequels = ""
			sequels = ""
			for aid in relations:
				print aid + ":" + relations[aid]
				if int(relations[aid]) == prequel:
					prequels += "%s (%s)," % (self.udp.get_anime(aid)["romaji"], aid) #replace udp lookup by parsing the xml file and getting the data from there.
				elif int(relations[aid]) == sequel:
					sequels += "%s (%s)," % (self.udp.get_anime(aid)["romaji"], aid)

			print prequels
			print sequels

			relation = ""
			if prequels != "":
				relation += "7 Prequel: " + prequels

			if sequels != "":
				relation += "7 Sequel: " + sequels

			return relation
		except:
			#no relation whatsoever
			return ""

	def set_secondary_title(self, data):
		if data["english"] != "" and data["english"] is not None:
			return data["english"]
		else:
			return data["kanji"]

	#####################################################
	#													#
	#					IRC OUTPUT						#
	#													#
	#####################################################

	#IRC Color code list
	#0 white
	#1 black
	#2 blue     (navy)
	#3 green
	#4 red
	#5 brown    (maroon)
	#6 purple
	#7 orange   (olive)
	#8 yellow
	#9 lt.green (lime)
	#10 teal    (a kinda green/blue cyan)
	#11 lt.cyan (cyan ?) (aqua)
	#12 lt.blue (royal)
	#13 pink    (light purple) (fuchsia)
	#14 grey
	#15 lt.grey (silver)

	def get_anime_output(self, data, arg):
		if data is not None:
			return u"4ANIME:7 %s (%s),11 also known as %s,12 %s eps, Year: %s,%s14 Categories: %s,12 Rating: %s (%s votes), Reviews: %s (avg: %s),14 More info: http://anidb.net/a%s" % (data["romaji"], data["aid"], self.set_secondary_title(data), data["eps"], self.check_year(data["year"]), self.set_anime_relations(data), self.check_categories(data["category"]), self.convert_vote(data["rating"]), data["vote_count"], data["review_count"], self.convert_vote(data["review_rating"]), data["aid"])
		else:
			return u"4ANIME: 7no such anime: 14%s" % (arg)

if __name__ == "__main__":
	bot = ircbot(irc_channel, irc_nickname, irc_server, irc_port)
	if bot.connect_to_anidb():
		bot.start()