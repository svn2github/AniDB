import os, sys, amara, datetime, copy

__out = "../../flat/"

def cssmerge(fullpath, outfile):
	path, filename = os.path.split(os.path.normpath(fullpath))
	infile = file(os.path.join(path,filename), 'rU')
	skip = 0
	incurly = 0
	nr = 0
	for line in infile:
		nr += 1
		line = line.strip()
		
		if line.count("{")>0:
			if incurly :
				print "ERROR: double curly @ "+fullpath+":"+`incurly`+"-"+`nr`
				sys.exit()
			else : incurly = nr

		if line.count("}")>0:
			if incurly : incurly = 0
			else:
				print "ERROR: negative curly @ "+fullpath+":"+`nr`
				sys.exit()
	
		if line.startswith("@import"):
			monkey = line[line.find('"')+1:line.rfind('"')]
			outfile.write("/*"+monkey+"*/\n")
			cssmerge(os.path.join(path,monkey), outfile)
		elif line.endswith("}"):
			outfile.write(line+"\n\n")
		elif line != "":
			outfile.write(line+"\n")
	infile.close()

def cssm(cssfile="/"):
	if os.path.exists(__out) is False:
		os.mkdir(__out)

	for line in file('stylelist','rU').readlines():
		if not line.startswith('#') and line.find(cssfile) > 0:
			path,name = line.rstrip('\n').rsplit('/',1)
			xml(unicode(path.lstrip('./').replace('/','-')),path)
			out = __out+path.lstrip('./').replace('/','-')
			if os.path.exists(out) is False:
				os.mkdir(out)
			cssmerge(line.rstrip('\n'),file(out + '/' + path.lstrip('./').replace('/','-') + '.css', 'w'))

def xml(newstyle,path):
	svn = {'day':18,'month':5,'year':2007} #date of the svn start. less bollocks now
	stylelist = []
	newest = 0
	new = {'status': u'', 'description': u'', 'creator': u'', 'update': u'', 'title': u'', 'path': u'', 'screenshot': u'', 'thumbnail': u''}
	descpath = path + '/' + 'description'
	xmldoc = amara.parse('./stylelist.xml')

	if os.path.exists(descpath):
		stuff = file(descpath, 'r').readlines()
		for line in stuff:
			line = line.rstrip('\n')
			if line.find(':') >0:
				key, val = line.split(':', 1)
				new[key] = unicode(val.strip())
			else:
				if new[key] == '':
					new[key] = new[key] + line
				else:
					new[key] = new[key] + '\n' + line

	for elem in ('thumbnail', 'screenshot'):
		if os.path.exists(os.path.join(path,'images',elem+'.png')):
			new[elem] = unicode(newstyle + '/' + 'images' +'/' +elem+'.png')
		else:
			new[elem] = u'none'
	
	for filename in os.listdir(path):
		if filename.endswith('.css'):
			mtime = os.path.getmtime(path + '/' + filename)
			if mtime >  newest:
				newest = mtime

	newfile = datetime.datetime.fromtimestamp(newest)

	if newfile.day >= svn['day'] and newfile.month >= svn['month'] and newfile.year >= svn['year']:
		new['update'] = unicode(newfile.strftime('%d.%m.%Y'))

	xmlstyles = xmldoc.css_styles.xml_xpath("style/@name")
	for i in range(len(xmlstyles)):
		stylelist += [xmlstyles[i].value]

	if newstyle in stylelist:
		styleelem = xmldoc.css_styles.style
		for i in range(len(styleelem)):
			if styleelem[i].name == newstyle:
				styleelem[i].title = new['title']
				styleelem[i].creator = new['creator']
				styleelem[i].update = new['update']
				styleelem[i].status = new['status']
				styleelem[i].description = new['description']
				styleelem[i].path = new['path']
				styleelem[i].thumbnail = new['thumbnail']
				styleelem[i].screenshot = new['screenshot']
	else:
		temp = copy.deepcopy(xmldoc.css_styles.style)
		temp.name = newstyle
		temp.title = new['title']
		temp.creator = new['creator']
		temp.update = new['update']
		temp.status = new['status']
		temp.description = new['description']
		temp.path = new['path']
		temp.thumbnail = new['thumbnail']
		temp.screenshot = new['screenshot']
		xmldoc.css_styles.xml_append(temp)

	output = file('./stylelist.xml', 'w')
	output.write(xmldoc.xml())

if __name__ == "__main__":
	cssm()
	print "done"
