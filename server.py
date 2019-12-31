import socket
import select

NUM_CONS = 2
catpaw = [None]*NUM_CONS
client = [None]*NUM_CONS
addr = [None]*NUM_CONS
print(client)
port = 8080 
for i in range(NUM_CONS):
    catpaw[i] = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    catpaw[i].bind(('192.168.0.107', port ))
    catpaw[i].listen(0)
    port+=1
    
print("Listening")

for i in range(NUM_CONS):
    client[i], addr[i] = catpaw[i].accept()
    print(f'{"Connected "}{i}')
    
while True:
    for i in range(1):
        client[0].send(b'a\n')
    
    readable, writable, exceptional = select.select(client, [], [])
    for i in range(len(readable)):
        content = readable[i].recv(8)
        if len(content)<0:
            print("Broken con")
            break        
        print(int(content[0]))
    
        client[int(content[0])].send(b'a\n')    
    

##send content if statements based on revieved data

print("Someone disconnected")
