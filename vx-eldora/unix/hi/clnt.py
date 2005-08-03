import socket
from DispStruct import *


host = 'vxtest'
port = 3000

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

ip = socket.gethostbyname(host)

ds = DispStruct(0)

ds.setMax0(1.2)

counter = 0

while 1:
    ds.setCmd(counter)
    s = ds.getString()
    sock.sendto(s,(host,port))
    counter = counter + 1
    ans = raw_input()
    if ans == 'q':
	break
