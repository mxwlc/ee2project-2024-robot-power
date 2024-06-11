import socket

def send_command(command):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', 5001))
    client_socket.send(command.encode())
    client_socket.close()

# Map buttons to commands
command_map = {
    "forward": "forward",
    "left": "left",
    "backward": "backward",
    "right": "right",
    "stop": "stop"
}

if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        command = sys.argv[1]
        if command in command_map:
            send_command(command_map[command])
        else:
            print("Invalid command")
    else:
        print("Usage: python rover_control.py <command>")
