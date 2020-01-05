import socket
import select

NUM_CONS = 3
catpaw = [None] * NUM_CONS
client = [None] * NUM_CONS
addr = [None] * NUM_CONS
print(client)
port = 8080
for i in range(NUM_CONS):
    catpaw[i] = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    catpaw[i].bind(('192.168.0.107', port))
    catpaw[i].listen(0)
    port += 1

print("Listening")

for i in range(NUM_CONS):
    client[i], addr[i] = catpaw[i].accept()
    print(f'{"Connected "}{i}')

input("Press enter to begin the game")
client[0].send(b'a\n')

while True:

    readable, writable, exceptional = select.select(client, [], [])
    for i in range(len(readable)):
        content = readable[i].recv(64)
        if len(content) < 0:
            print("Broken con")
            break
        print(int(content[0]))

        if content[0] < NUM_CONS:
            client[int(content[0])].send(b'a\n')


# send content if statements based on revieved data

print("Someone disconnected")
