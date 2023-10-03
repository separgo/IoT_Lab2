import socket
#import os
from _thread import *

ServerSocket = socket.socket()
host = '192.168.1.79'
port = 10000

distance = -1;

try:
    ServerSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Waiting for a Connection...')
ServerSocket.listen(5)

def threaded_client(connection):
    # connection.send(str.encode('Welcome to the Server\n'))
    while True:
        global distance
        data = connection.recv(2048).decode('utf-8')
        if data.startswith('distance=') :
            distance = int(data[9:-1])
            print('distance=' + str(distance))
            reply = "ACK\n"
        elif data.startswith('GET') :
            if distance < 0 :
                reply = "NOT DEFINED\n"
            elif distance < 10 :
                reply = "RED\n"
            elif distance < 20 :
                reply = "YELLOW\n"
            else :
                reply = "GREEN\n"
        if not data:
            break
        #connection.sendall(reply.encode('utf-8'))
    connection.close()

while True:
    Client, address = ServerSocket.accept()
    print('Connection from: ' + address[0] + ':' + str(address[1]))
    start_new_thread(threaded_client, (Client, ))
ServerSocket.close()

