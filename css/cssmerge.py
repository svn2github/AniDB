"""
To use the ftp feature you got to create a ftp.txt file above the level of
the cvs folder (1 level above the adm, cgi-bin, etc. folder) with the
following lines:
www.anidb.net
account
password
Furthermore to upload stuff to the ftp you have to run it with one of the following options:
- to upload the css and all pics newer than the last upload (timestamp in lastpicupload file on the ftp) use 'upload'
- to upload only the css use option 'cssupload'
- to upload the css and all pics use option 'fullupload'

default option should
"""

import os, sys, ftplib, amara, copy, datetime,time,urllib

__out = "../../flat/"
__ftp = {}
__ftppath = '/css/'
__extensions = ('.jpg', '.jpeg', '.gif', '.png','.css')

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
	for line in file('stylelist','rU').readlines():
		if not line.startswith('#'):
			path,name = line.rstrip('\n').rsplit('/',1)	
			xml(unicode(path.lstrip('./').replace('/','-')),path)
			out = __out+path.lstrip('./').replace('/','-')
			if os.path.exists(out) is False:
				os.mkdir(out)
			cssmerge(line.rstrip('\n'),file(out + '/' + path.lstrip('./').replace('/','-') + '.css', 'w'))
			__ftp[path] = path.lstrip('./').replace('/','-') + '.css'

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
				key, val = line.split(':')
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
			mtime = os.path.getmtime(path+'\\'+filename)
			if mtime >  newest:
				newest = mtime
				newestf = filename

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

def doftp(update):
	ftp_update = []
	if update in ('upload','fullupload'):
		try:
			lastupdate = int(urllib.urlopen('http://www.anidb.net/css/lastpicupload').read())
			file(__out + 'lastpicupload','w').write(str(lastupdate))
			ftp_update += [(__ftppath + 'lastpicupload',__out + 'lastpicupload')]
		except:
			file(__out + 'lastpicupload','w').write(str(time.mktime(time.localtime())))
			lastupdate = 0
			print "Couldn't determine last picture upload. Running fullupdate."
	for css in __ftp:
		ftp_update += [(__ftppath + css.lstrip('./').replace('/','-') + '/' + __ftp[css],__out+ css.lstrip('./').replace('/','-') + '/'+__ftp[css])]
		if update in ('upload','fullupload'):
			if os.path.exists(css+'/images'): #do we have an /image path for the css?
				for root,path,filename in os.walk(css+'/images'):
					root = root.replace('\\','/') #evil mixed \ and /
					for elem in filename:
						if filter(elem.lower().endswith, __extensions):
							if update == 'upload':
								fileupdate = os.path.getmtime(root + '/' + elem)
								filecreation = os.path.getctime(root + '/' + elem)
								if (lastupdate < fileupdate) or (lastupdate < filecreation):
									ftp_update += [(__ftppath + css.lstrip('./').replace('/','-') + '/images/' + elem,root + '/' + elem)]
							else:
								ftp_update += [(__ftppath + css.lstrip('./').replace('/','-') + '/images/' + elem,root + '/' + elem)]

	ftp_update += [(__ftppath + 'stylelist.xml','./stylelist.xml')]
	anidbftp = ftplib.FTP(*file("../../ftp.txt",'rU').read().split("\n"))

	for ftp_path, local_file in ftp_update:
		try:
			anidbftp.storbinary("STOR "+ftp_path, file(local_file,'rb'))
			print "Uploading", ftp_path
		except ftplib.error_perm, error:
			ftpfolder(ftp_path,anidbftp)
			try:
				anidbftp.storbinary("STOR "+ftp_path, file(local_file,'rb'))
				print "Uploading", ftp_path
			except ftplib.error_perm, error:
				print error
	anidbftp.quit()

def ftpfolder(ftp_path,anidbftp,position='/'):
	if ftp_path.find('/') >=0:
		root,path = ftp_path.lstrip('/').split('/',1)
		anidbftp.cwd('/')
		try:
			anidbftp.cwd(position)
			anidbftp.cwd(root)
		except:
			try:
				anidbftp.mkd(root)
			except ftplib.error_perm, error:
				print error
		ftpfolder(path,anidbftp,position+'/'+root)

if __name__ == "__main__":
	cssm()

	if len(sys.argv) > 1:
		doftp(sys.argv[1])
	print "done"
