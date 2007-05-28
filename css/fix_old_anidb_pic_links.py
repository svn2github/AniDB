'''
lil script to fix old anidb pic paths with the new ones
'''

import os

for root, dirs, filenames in os.walk(os.getcwd()):
    for cssfile in filenames:
        if cssfile.endswith('css'):
            newfile = ''
            css = file(root+'/'+cssfile,'rbU').readlines()
            for line in css:
                line = line.replace('div.icoco','div.icons')
                line = line.replace('http://www.anidb.net/css/anidbstyle/images/bg','../anidbstyle/images')
                line = line.replace('http://anidb.net/css/anidbstyle/images/bg','../anidbstyle/images')
                line = line.replace('http://www.anidb.net/css/anidbstyle/images','../icons')
                line = line.replace('http://anidb.net/css/anidbstyle/images','../icons')
                line = line.replace('http://www.anidb.net/pics','../icons/flags')
                line = line.replace('http://anidb.net/pics','../icons/flags')
                newfile +=line
            file(root+'/'+cssfile,'wbU').write(newfile)
