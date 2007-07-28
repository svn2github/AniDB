#!/usr/bin/env python
############################################################################
#    Copyright (C) 2007 by Corbin Simpson                                  #
#    MostAwesomeDude@gmail.com                                             #
#                                                                          #
#    This program is free software; you can redistribute it and/or modify  #
#    it under the terms of the GNU General Public License as published by  #
#    the Free Software Foundation; either version 2 of the License, or     #
#    (at your option) any later version.                                   #
#                                                                          #
#    This program is distributed in the hope that it will be useful,       #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#    GNU General Public License for more details.                          #
#                                                                          #
#    You should have received a copy of the GNU General Public License     #
#    along with this program; if not, write to the                         #
#    Free Software Foundation, Inc.,                                       #
#    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
############################################################################

import ConfigParser

class config:
	'''This class wraps ConfigParser without inheriting from it. The
	point is to have a semi-stateful and simple config class that keeps
	openanidb.py from worrying about silly things like config file
	locations or value conversion.'''
	def __init__(self):
		# Config file(s)
		self.files = ["openanidb.ini"]
		self.parser = ConfigParser.SafeConfigParser()
		self.parser.read(self.files)
		if not self.parser.has_section("OpenAniDB"):
			self.parser.add_section("OpenAniDB")
	
	def reload(self):
		self.parser.read(self.files)
		if not self.parser.has_section("OpenAniDB"):
			self.parser.add_section("OpenAniDB")
	
	def save(self):
		self.parser.write(file("openanidb.ini", "w"))
	
	def get(self, key):
		'''Gets key's value, or None if not set.'''
		try:
			retval = self.parser.get("OpenAniDB", key)
			if retval == "True":
				return True
			elif retval == "False":
				return False
			else:
				return retval
		except ConfigParser.NoOptionError:
			return None
	
	def set(self, key, value):
		'''Sets key to value.'''
		self.parser.set("OpenAniDB", key, str(value))