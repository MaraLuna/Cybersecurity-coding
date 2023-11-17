import sys, json
from socket import socket, SOCK_STREAM, AF_INET

max_byte_limit = 60

def print_error(e, f="UNKNOWN"):
    print("Error in %s!" % (f))
    print(e)
    print(type(e))

def connect_to_server(server_address, server_port):
    try:
        client_socket = socket(AF_INET, SOCK_STREAM)
        client_socket.connect((server_address, server_port))
        return client_socket
    except Exception as e:
        print("Error connecting to the server:", str(e))
        return None

def send_message(tcp_sock, data):
    try:
        json_data = json.dumps(data)
        ret = tcp_sock.send(json_data.encode('utf-8'))
        print("Sent %d bytes" % ret)
    except KeyboardInterrupt as k:
        raise KeyboardInterrupt()
    except Exception as e:
        print("Error sending data:", str(e))

def recv_message(tcp_sock):
    try:
        data=tcp_sock.recv(4096)
        if not data:
            print("No data received.")
            return None
        return json.loads(data.decode('utf-8'))
    except Exception as e:
        print("Error receiving message:", str(e))
        return None

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 client.py <server_ip> <server_port>")
        sys.exit(1)
    server = sys.argv[1]
    print (server)
    port = int(sys.argv[2])
    print(port)
    client_socket = connect_to_server(server,port)
    try:
        data = None
        if client_socket:
            active = True
            while active:
                name = input("Please enter your name: ")
                name = name
                name_b = len(name.encode('utf-8'))
                print("Name in bytes = ", name_b)
                if name_b > 59:
                    print("***** Error - name is too long ******")
                else:
                    active = False
            active2 = True
            targets=[]
            while active2:
                target_input = input("Enter the rooms you would like to listen to separated by commas: ")
                targets=target_input.split(',')
                print(targets)
                for target in targets:
                    target_size_bytes = len(target.encode('utf-8'))
                    if target_size_bytes > 59:
                        print(f"Room '{target}' is larger than 60 bytes.")
                else:
                    active2 = False

            connect_message = {
                "action": "connect",
                "user_name": "@" + name,
                "targets": ["#" + t for t in targets]
            }
            print (json.dumps(connect_message))
            send_message(client_socket, connect_message)

            while True:
                message = input("Enter your message (maximum 3800 bytes or exit to quit): ")

                if message.lower() == 'exit':
                    # Send a "disconnect" message to the server and exit the loop
                    disconnect_message = {
                        "action": "disconnect"
                    }
                    send_message(client_socket, disconnect_message)
                    break

                target = input("Enter the target room/user: ")
                # Check if the message size is within the limit (3800 bytes)
                message_size_bytes = len(message.encode('utf-8'))
        
                if message_size_bytes > 3800:
                    print("Message size exceeds the 3800-byte limit. Please enter a shorter message.")
                    continue

                # Send a "message" message to the server
                message_data = {
                    "action": "message",
                    "user_name": "@" + name,
                    "target": '#' + target,
                    "message": message
                }
                send_message(client_socket, message_data)

                # Receive and display messages from the server
                server_message = recv_message(client_socket)
                print("Received message from the server:", server_message)  # Debugging line
                if server_message:
                    if server_message["status"] == "chat":
                        for chat_message in server_message["history"]:
                            chat_message_dict = json.loads(chat_message)
                        print(f"[{chat_message_dict['target']}]: {chat_message_dict['message']}")
                    elif server_message["status"] == "error":
                        print(f"Server Error: {server_message['message']}")
                
    except KeyboardInterrupt:
        print("User interrupted the program.")
    except Exception as e:
        print("An error occurred:", str(e))
    finally:
        # Close the socket in the finally block to ensure it's closed even if an exception occurs
        if client_socket:
            client_socket.close()
            print("Socket closed.")
    
if __name__ == "__main__":
    main()
