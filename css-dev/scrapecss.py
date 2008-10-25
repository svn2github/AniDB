'''
lil css pic scraper
-------------------
feed it a file (scrapecss.txt) with links to css and it will look for "url("
and download the files behind those.
download path is set in __base. so set accordingly

+ follows @import lines
+ works also with relative paths in the css files (tries to build the path)
- It's not meant to work with local files. It could be changed to handle those, but
that's hardly needed. if at lal only for tetsing purposes
'''

import os
from urllib2 import urlopen

__base = 'D:/test'
__pics = []

def create_folder(path,position='/'):
    if path.find('/') >=0:
        root,path = path.lstrip('/').split('/',1)
        if not os.path.exists(__base+position+'/'+root):
            os.chdir(__base+position)
            os.mkdir(root)
        create_folder(path,position+'/'+root)

def get_css(link):
    print 'now scraping', link
    if link.startswith('http://'):
        css = urlopen(link)
        get_url(css,link)
    else:
        print 'ERROR - Expected url, got something else:', link

def get_url(css,link):
    for line in css.readlines():
        if line.find('@import')>=0:
            if not line.startswith('/*'):
                i = line.find('"') + 1
                j = line.rfind('"')
                stuff = line[i:j].replace("'","").replace('"','')
                if not stuff.find(':') >0:
                    root,crap = link.rsplit('/',1)
                    newlink = root + '/' + stuff
                get_css(newlink)
            
        if line.find('url(')>0:
            i = line.find('(') + 1
            j = line.find(')')
            stuff = line[i:j].replace("'","").replace('"','')
            if not stuff.startswith('http://'):
                root,crap = link.rsplit('/',1)
                stuff = root + '/' + stuff
            __pics.append(stuff)

def remove_dupes():
    __pics.sort()
    for elem in __pics:
        if __pics.count(elem) >1:
            __pics.remove(elem)
    for url in __pics:
        download(url)

def download(url):
    try:
        pic = urlopen(url,'r')
        try:
            file(__base +'/'+ url.lstrip('http://'),'wb').write(pic.read())
            print 'Downloading', url
        except:
            create_folder(url.lstrip('http://'))
            download(url)
    except:
        print 'ERROR - url not valid or file not existing:', url

def read_stuff_to_do():
    scrape = file('scrapecss.txt','r')
    for line in scrape:
        if line != '':
            get_css(line.rstrip('\n'))

if __name__ == "__main__":
    read_stuff_to_do()
    remove_dupes()
