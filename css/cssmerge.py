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

prep = "sub/"
prep2 = "bits/"
out = "../../flat/"

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

def cssm(fname,subd):
	if os.path.exists(out) == False:
		os.mkdir(out)

	outfile = file(out+fname, 'w')
	cssmerge(subd+fname, outfile)
	outfile.close()

def docss():
	cssm("sub-silver.css",prep)
	cssm("sub-blue.css",prep)
	cssm("sub-brown.css",prep)
	cssm("sub-dragon.css",prep)
	cssm("sub-gray.css",prep)
	cssm("sub-norwegian-blue.css",prep)
	cssm("anidbstyle.css",prep2)
	cssm("blue.css",prep2)
	cssm("brown2.css",prep2)
	cssm("suppy.css",prep2)

def doftp():
	ftp_update = [
		("css/sub/silver.css", out+"sub-silver.css"),
		("css/sub/blue.css", out+"sub-blue.css"),
		("css/sub/brown.css", out+"sub-brown.css"),
		#("css/sub/dragon.css", out+"sub-dragon.css"),
		("css/sub/gray.css", out+"sub-gray.css"),
		("css/sub/norwegian-blue.css", out+"sub-norwegian-blue.css"),
		("css/anidbstyle/anidbstyle.css", out+"sub-gray.css"),
		("css/blue/blue.css", out+"sub-norwegian-blue.css"),
		#("css/brown2/brown2.css", out+"brown2.css"),
		#("css/suppy.css", out+"suppy.css")
		("css/icons-flags.css", "base/icons-flags.css"),
	]

	anidbftp = ftplib.FTP(*file("../../ftp.txt").read().split("\n"))
	for ftp_path, local_file in ftp_update:
		print "Uploading", ftp_path
		anidbftp.storbinary("STOR "+ftp_path, file(local_file))
	anidbftp.quit()

if __name__ == "__main__":
	docss()
	
	if sys.argv.pop() == "upload" :
		doftp()

	print "done"
