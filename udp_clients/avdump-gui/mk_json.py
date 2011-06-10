import os, md5, json

def get_md5(filename):
    checksum = md5.new()
    f = file(filename, 'rb')

    while True:
        part = f.read(1024)
        if not part:
            break # end of file
        checksum.update(part)

    f.close()
    return checksum.hexdigest()

avd2files = []
for root, dir, files in os.walk('./dist'):
    for filename in files:
        checksum = get_md5(os.path.join(root, filename))
        avd2files.append({'name': filename, 'md5': checksum})

print json.dumps({'version': '2.02', 'files': avd2files}, sort_keys=True)
        
        
        
