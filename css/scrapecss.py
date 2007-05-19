'''
lil css pic scraper
-------------------
feed it a file (scrapecss.txt) with links to css and it will look for "url("
and download the files behind those. works also with relative pathes.
download path is set in __base. so set accordingly
'''

import os
from urllib import urlopen

__base = 'D:/test'

def create_folder(path,position='/'):
    if path.find('/') >=0:
	root,path = path.lstrip('/').split('/',1)
        if not os.path.exists(__base+position+'/'+root):
            os.chdir(__base+position)
            os.mkdir(root)
        create_folder(path,position+'/'+root)

def get_url(link):
    print 'now scraping', link
    if link.startswith('http://'):
        css = urlopen(link)
    else:
        css = file(link)

    for line in css.readlines():
        if line.find('url(')>0:
            i = line.find('(') + 1
            j = line.find(')')
            stuff = line[i:j].replace("'","").replace('"','')
            if not stuff.startswith('http://www.anidb.net'):
                if not stuff.startswith('http://'):
                    root,crap = link.rsplit('/',1)
                    stuff = root+'/'+stuff
                download(stuff)

def download(url):
    print 'Downloading', url
    pic = urlopen(url)
    try:
        file(__base +'/'+ url.lstrip('http://'),'wb').write(pic.read())
    except:
        create_folder(url.lstrip('http://'))
        download(url)

def read_stuff_to_do():
    scrape = file('scrapecss.txt','r')
    for line in scrape:
        if line != '':
            get_url(line.rstrip('\n'))

if __name__ == "__main__":
    read_stuff_to_do()
