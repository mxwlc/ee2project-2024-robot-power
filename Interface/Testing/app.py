from flask import Flask, render_template, request, redirect, url_for
import socket
import threading

app = Flask(__name__)

# Global data to simulate real-time updates
data = {
    'batteryLevel': 85,
    'powerConsumption': 12,
    'speed': 3.5
}

def update_data():
    while True:
        data['batteryLevel'] = max(0, data['batteryLevel'] - 0.1)
        data['powerConsumption'] = 12  # Simulated data
        data['speed'] = 3.5  # Simulated data
        time.sleep(1)

def socket_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 5001))
    server_socket.listen(5)

    while True:
        client_socket, addr = server_socket.accept()
        command = client_socket.recv(1024).decode()
        print(f'Received command: {command}')
        # Handle the command here
        client_socket.close()

@app.route('/')
def index():
    return render_template('interface.html', data=data)

if __name__ == '__main__':
    threading.Thread(target=update_data).start()
    threading.Thread(target=socket_server).start()
    app.run(debug=True, port=5000)
