'''
lil script to fix old anidb pic paths with the new ones
don't trust this too much. changed files should get checked manually
with a diff tool afterwards
'''

import os

for root, dirs, filenames in os.walk(os.getcwd()):
    for cssfile in filenames:
        if cssfile.endswith('css'):
            newfile = ''
            css = file(root+'/'+cssfile,'rb').readlines()
            for line in css:
                line = line.replace('../../../images','images')
                line = line.replace('../../images','images')
                line = line.replace('../images','images')
                newfile +=line
            file(root+'/'+cssfile,'wb').write(newfile)
