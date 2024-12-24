#!/usr/bin/python

import sys
import string
import socket
from time import sleep

PYVER = sys.version_info.major

if PYVER == 3:
    data = string.digits + string.ascii_lowercase + string.ascii_uppercase
else:
    data = string.digits + string.lowercase + string.uppercase

def server(port):
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    s.bind(('0.0.0.0', int(port)))
    s.listen(3)
    
    cs, addr = s.accept()
    print(addr)
    
    f = open('server-output.dat', 'wb')
    
    while True:
        data = cs.recv(1000)
        print (len(data))
        f.write(data)
        
        if data:
            if PYVER == 3:
                #data = "server echoes: " + data.decode()
                data = "server echoes: recv ok"
                cs.send(data.encode())
            else:
                #data = 'server echoes: ' + data
                data = 'server echoes: recv ok'
                cs.send(data)
        else:
            break

    f.close()
    s.close()


def client(ip, port):
    s = socket.socket()
    s.connect((ip, int(port)))
    
    f = open('client-input.dat', 'r')
    file_str = f.read()
    length = len(file_str)
    i = 0
    '''
    for i in range(10):
        new_data = data[i:] + data[:i+1]
        if PYVER == 3:
            s.send(new_data.encode())
            print(s.recv(1000).decode())
        else:
            s.send(new_data)
            print(s.recv(1000))
        sleep(1)
    '''
    while length > 0:
        send_len = min(length, 10000)
        new_data = file_str[i: i+send_len]
        if PYVER == 3:
            s.send(new_data.encode())
            sleep(1)
            print(s.recv(1000).decode())
        else:
            s.send(new_data)
            sleep(1)
            print(s.recv(1000))
        length -= send_len
        i += send_len
    
    f.close()
    s.close()

if __name__ == '__main__':
    if sys.argv[1] == 'server':
        server(sys.argv[2])
    elif sys.argv[1] == 'client':
        client(sys.argv[2], sys.argv[3])
