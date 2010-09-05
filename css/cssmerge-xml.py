import os, copy, datetime, sys
from lxml import etree

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
		
		#if line.count("{")>0:
		#	if incurly :
		#		print "ERROR: double curly @ "+fullpath+":"+`incurly`+"-"+`nr`
		#		sys.exit()
		#	else : incurly = nr

		#if line.count("}")>0:
		#	if incurly : incurly = 0
		#	else:
		#		print "ERROR: negative curly @ "+fullpath+":"+`nr`
		#		sys.exit()

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
	tree = etree.parse('stylelist.xml')

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

	xmlstyles = tree.getroot().xpath("style/@name")
	for i in range(len(xmlstyles)):
		stylelist += [xmlstyles[i]]

	if newstyle in stylelist:
		styleelem = list(tree.getroot())
		for i in range(len(styleelem)):
			if styleelem[i].get("name") == newstyle:
				anime = styleelem[i]
				anime.iterchildren(tag="title").next().text = new['title']
				anime.iterchildren(tag="creator").next().text = new['creator']
				anime.iterchildren(tag="update").next().text = new['update']
				anime.iterchildren(tag="status").next().text = new['status']
				anime.iterchildren(tag="description").next().text = new['description']
				anime.iterchildren(tag="path").next().text = new['path']
				anime.iterchildren(tag="thumbnail").next().text = new['thumbnail']
				anime.iterchildren(tag="screenshot").next().text = new['screenshot']
	else:
		anime = etree.SubElement(tree.getroot(), "style")
		anime.set("name", newstyle)
		set_node(anime, "title", new['title'])
		set_node(anime, "creator", new['creator'])
		set_node(anime, "update", new['update'])
		set_node(anime, "status", new['status'])
		set_node(anime, "description", new['description'])
		set_node(anime, "path", new['path'])
		set_node(anime, "thumbnail", new['thumbnail'])
		set_node(anime, "screenshot", new['screenshot'])

	output = file('stylelist.xml', 'w')
	indent(tree.getroot())
	output.write(etree.tostring(tree, pretty_print=True, encoding="UTF-8", xml_declaration=True))

def set_node(node, tag, content):
	tmp = etree.SubElement(node, tag)
	tmp.text = content

def indent(elem, level=0):
	i = "\n" + level*"\t"
	if len(elem):
		if not elem.text or not elem.text.strip():
			elem.text = i + "\t"
		for e in elem:
			indent(e, level+1)
			if not e.tail or not e.tail.strip():
				e.tail = i + "\t"
		if not e.tail or not e.tail.strip():
			e.tail = i
	else:
		if level and (not elem.tail or not elem.tail.strip()):
			elem.tail = i

if __name__ == "__main__":
	cssm()
	print "done"
