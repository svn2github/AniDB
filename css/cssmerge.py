"""
To use the ftp feature you got to create a ftp.txt file above the level of
the cvs folder (1 level above the adm, cgi-bin, etc. folder) with the
following lines:
www.anidb.net
account
password
Furthermore you have to run the script with the option "upload"
"""

import os, sys, ftplib

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

def doftp(ftp_update = []):
	for css in __ftp:
		ftp_update += [(__ftp[css],__out+css)]

	anidbftp = ftplib.FTP(*file("../../ftp.txt").read().split("\n"))
	for ftp_path, local_file in ftp_update:
		print "Uploading", ftp_path
		anidbftp.storbinary("STOR "+ftp_path, file(local_file))
	anidbftp.quit()

if __name__ == "__main__":
	cssm()
	
	if sys.argv.pop() == "upload" :
		doftp()

	print "done"
