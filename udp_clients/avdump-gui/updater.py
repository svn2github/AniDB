import os, urllib2, traceback, json, md5, httplib, sys, urllib

class updater():
    def __init__(self):
        self._base_url    = 'http://static.anidb.net/client/avdump2/'
        self._version_url = self._base_url + 'update.json'

    def _get_response(self, url):
        try:
            request = urllib2.urlopen(url)
            return request.read()    
        except urllib2.HTTPError, e:
            print 'Unable to get latest version info - HTTPError = ' + str(e.reason)
            sys.exit(2)
        except urllib2.URLError, e:
            print 'Unable to get latest version info - URLError = ' + str(e.reason)
            sys.exit(2)    
        except httplib.HTTPException, e:
            print 'Unable to get latest version info - HTTPException'
            sys.exit(2)    
        except Exception, e:
            print 'Unable to get latest version info - Exception = ' + traceback.format_exc()
            sys.exit(2)

    def _download_file(self, filename):
        try:
            return urllib.urlretrieve(self._base_url + "current/" + filename)
        except urllib2.HTTPError, e:
            print 'Unable to get latest version info - HTTPError = ' + str(e.reason)
            sys.exit(2)
        except urllib2.URLError, e:
            print 'Unable to get latest version info - URLError = ' + str(e.reason)
            sys.exit(2)    
        except httplib.HTTPException, e:
            print 'Unable to get latest version info - HTTPException'
            sys.exit(2)    
        except Exception, e:
            print 'Unable to get latest version info - Exception = ' + traceback.format_exc()
            sys.exit(2)

    def _get_md5(self, filepath):
        checksum = md5.new()
        f = file(filepath, 'rb')

        while True:
            part = f.read(1024)
            if not part:
                break # end of file
            checksum.update(part)
        f.close()
        return checksum.hexdigest()

    def _md5_is_valid(self, org_md5, filename):
        new_md5 = self._get_md5(filename)
        #print filename, org_md5, new_md5
        if org_md5 == new_md5:
            return True
        else:
            return False

    def _update_files(self, files):
        for avd2file in files:
            if os.path.exists(agentFile['name']):
                os.remove(agentFile['name'])

            os.rename(agentFile['tempFile'], agentFile['name'])

    def _get_updateInfo(self):
        response = self._get_response(self._version_url)
        if response is not None:
            return json.loads(response)
        else:
            return None

    def _get_localInfo(self):
        locfiles = {}
        for root, dir, files in os.walk('./dist'):
            for filename in files:
                checksum = self._get_md5(os.path.join(root, filename))
                locfiles[filename] = checksum
        return locfiles
       
    def _update_file(self, filename, checksum):
        downloadedfile = self._download_file(filename)[0]
        print filename, downloadedfile#, checksum
        if self._md5_is_valid(checksum, downloadedfile) is True:
            orgfile = os.path.join(os.getcwd(), 'dist', filename)
            if os.path.exists(orgfile):
                os.remove(orgfile)
            print downloadedfile, orgfile
            os.rename(downloadedfile, orgfile)

    def check_version(self):
        localfiles = self._get_localInfo()
        updateInfo = self._get_updateInfo()
        if updateInfo is None:
            print 'ERROR'
            return

        for avd2file in updateInfo['files']:
            if avd2file['name'] in localfiles.keys():
                if localfiles[avd2file['name']] != avd2file['md5']:
                    self._update_file(avd2file['name'], avd2file['md5'])
            else:
                self._update_file(avd2file['name'], avd2file['md5'])

if __name__ == "__main__":
    upd = updater()
    upd.check_version()
