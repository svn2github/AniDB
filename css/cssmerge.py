"""
To use the ftp feature you got to create a ftp.txt file above the level of
the cvs folder (1 level above the adm, cgi-bin, etc. folder) with the
following lines:
www.anidb.net
account
password
Furthermore you have to run the script with the option "upload"
"""

import os, sys, ftplib, amara, copy

__out = "../../flat/"
__ftp = {}

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

def cssm():
	if os.path.exists(__out) is False:
		os.mkdir(__out)
	for line in file('stylelist').readlines():
		if not line.startswith('#'):
			path,name = line.rstrip('\n').rsplit('/',1)
			oname = path.lstrip('./').replace('/','-') + '.css' #evil shite because of epoxis main.css ;_;
			__ftp[oname] = path
			cssmerge(line.rstrip('\n'),file(__out+oname, 'w'))
			xml(unicode(oname.rstrip('.css')),path)

def xml(newstyle,path):
	__stylelist = []
	xmldoc = amara.parse('./stylelist.xml')
	new = {'status': u'', 'description': u'', 'creator': u'', 'update': u'', 'title': u''}

	if os.path.exists(path + '/' + 'description'):
		stuff = file(path + '/' + 'description', 'r').readlines()
		for line in stuff:
			key, val = line.split(':')
			new[key] = unicode(val.strip())

	for i in range(len(xmldoc.css_styles.xml_xpath("style/@name"))):
		__stylelist += [xmldoc.css_styles.xml_xpath("style/@name")[i].value]

	if newstyle in __stylelist:
		for i in range(len(xmldoc.css_styles.style)):
			if xmldoc.css_styles.style[i].name == newstyle:
				xmldoc.css_styles.style[i].title = new['title']
				xmldoc.css_styles.style[i].creator = new['creator']
				xmldoc.css_styles.style[i].update = new['update']
				xmldoc.css_styles.style[i].status = new['status']
				xmldoc.css_styles.style[i].description = new['description']
	else:
		temp = copy.deepcopy(xmldoc.css_styles.style)
		temp.name = newstyle
		temp.title = new['title']
		temp.creator = new['creator']
		temp.update = new['update']
		temp.status = new['status']
		temp.description = new['description']
		xmldoc.css_styles.xml_append(temp)

	output = file('./stylelist.xml', 'w')
	output.write(xmldoc.xml())

def doftp(ftp_update = []):
	for css in __ftp:
		ftp_update += [('css/' + __ftp[css].lstrip('./') + '/' + css,__out+css)]
		if os.path.exists(__ftp[css]+'/images'): #do we have an /image path for the css?
			for root,path,filename in os.walk(__ftp[css]+'/images'):
				root = root.replace('\\','/') #evil mixed \ and /
				for elem in filename:
					if elem.endswith('.gif') or elem.endswith('.jpg') or elem.endswith('.png'):
						ftp_update += [('css/' + root.lstrip('./') + '/' + elem,root + '/' + elem)]

	ftp_update += [('css/','./stylelist.xml')]
	
	anidbftp = ftplib.FTP(*file("../../ftp.txt").read().split("\n"))
	for ftp_path, local_file in ftp_update:
		try:
		    anidbftp.storbinary("STOR "+ftp_path, file(local_file))
		    print "Uploading", ftp_path
		except:
		    print 'Some error while uploading', ftp_path #needs soem real fixing for when the folder doesn't exist
	anidbftp.quit()             

if __name__ == "__main__":
	cssm()
	
	if sys.argv.pop() == "upload" :
		doftp()

	print "done"
