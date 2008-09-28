# Made by Deepy of www.armchairs.be
# And www.snakeporno.com
# Licensed under the FUCK YOU GPL License.
# You may not remove or modify this text. EVER!

# You may however do whatever you want with the rest of the code, as long 
# as you do not license it under the GPL.
# I made this with PICO and not NANO.
import socket, sys, time, string

#host = sys.argv[1]
host = "api.anidb.info"
textport = "9000"

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
port = int(textport)

s.connect((host, port))
#print "Enter data to transmit: "
#data = sys.stdin.readline().strip()
# You may want to add your username and such here.
data = "AUTH user=yourusername&pass=yourpassword&protover=3&client=deap&clientver=0"
s.sendall(data)
#s.shutdown(1)
print "Looking for replies; press Ctrl-C or Ctrl-Break to stop."
while 1:
    buf = s.recv(2048)
    if not len(buf):
        break
    myrecv = buf.split(' ', 2)
    if myrecv[0] == '201':
	print 'New version available'
    elif myrecv[0] == '500':
	print 'Login failed!'
	break
    print "Received: %s" % buf
    print myrecv[1]
    mylogout = "LOGOUT s=%s" % myrecv[1]
    s.sendall(mylogout)
    buf = s.recv(2048)
    if not len(buf):
        break
    print buf
    s.shutdown(1)
    break
