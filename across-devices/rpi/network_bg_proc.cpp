#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <poll.h>
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <chrono>
#include <thread>

#define PORT 9876
#define MAX_RECV_BUFFER_SIZE 8192
#define MAX_SEND_BUFFER_SIZE MAX_RECV_BUFFER_SIZE

class WirelessConnection {

    int listener_fd;
    volatile int client_fd;
    sockaddr_in client;
    volatile bool is_connected;
    // pollfd send_poll, recv_poll; // Should this be volatile? It does cause an error

    void connectUser();

public:
    WirelessConnection();
    ~WirelessConnection();

    void sendData(const char* data, int len);
    int recvData(char* buffer, int expected_len);

};

void WirelessConnection::connectUser() {
    std::cout << "[NETWORK BG] Listening for client" << std::endl;

    // Listen for client
    listen(listener_fd, 1);

    // Accept new client, stop listening
    int tmp = sizeof(client);
    client_fd = accept(listener_fd, (sockaddr *) &client, (socklen_t *) &tmp);
    listen(listener_fd, 0);
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt(... SO_RCV_TIMEO...)");
        exit(1);
    }
    if (setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt(... SO_SND_TIMEO...)");
        exit(1);
    }
    int timeout2 = 1000;
    if (setsockopt(client_fd, SOL_TCP, TCP_USER_TIMEOUT, &timeout2, sizeof(timeout2)) == -1) {
        perror("setsockopt(... TCP_USER_TIMEOUT...)");
        exit(1);
    }

    is_connected = true;
    // send_poll.fd = client_fd;
    // recv_poll.fd = client_fd;

    std::cout << "[NETWORK BG] Connected new client" << std::endl;
}

WirelessConnection::WirelessConnection() : is_connected(false) {
    // Only hosting - join occurs in Python

    // Create socket
    listener_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Create address object
    sockaddr_in addr;
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // Bind socket to address and port
    bind(listener_fd, (sockaddr*) &addr, sizeof(addr));

    // send_poll.events = POLLOUT;
    // recv_poll.events = POLLIN;

    connectUser();
}

WirelessConnection::~WirelessConnection() {
    close(listener_fd);
    close(client_fd);
}

void WirelessConnection::sendData(const char* data, int len) {
    while (len > 0) {
        int cur_send_len;
        if (len > MAX_SEND_BUFFER_SIZE) cur_send_len = MAX_SEND_BUFFER_SIZE;
        else cur_send_len = len;

        // Note: this function completely freezes if a client disconnects during the execution
        // of send(...). A timeout does not occur, and occasionally this causes the receiving
        // thread to also freeze and wait. Using poll() before calling send() didn't help. It also
        // prevents the receiving thread from listening for a new client, potentially due to
        // blocking the socket from being closed. While Linux does forcibly close the program
        // due to a broken pipe, it still requires a restart of the Pi when it happens.

        if (!is_connected) return;
        // poll(&send_poll, 1, 1000);
        // if (!(send_poll.revents & POLLOUT)) return;
        int send_size = send(client_fd, data, cur_send_len, 0);
        while (send_size == -1) {
            std::cout << "[NETWORK BG] Error while sending:" << std::endl;
            perror("send");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (!is_connected) return;
            // poll(&send_poll, 1, 1000);
            // if (!(send_poll.revents & POLLOUT)) return;
            send_size = send(client_fd, data, cur_send_len, 0);
        }
        data += cur_send_len;
        len -= cur_send_len;
    }
}

int WirelessConnection::recvData(char* buffer, int expected_len) {
    int recv_len = 0;
    while (recv_len < expected_len) {
        // Get the current write target within buffer
        char* tmp_buffer = buffer + recv_len;

        // Receive the amount of data to write
        int cur_max_recv;
        if ((expected_len - recv_len) > MAX_RECV_BUFFER_SIZE) cur_max_recv = MAX_RECV_BUFFER_SIZE;
        else cur_max_recv = expected_len - recv_len;

        /*poll(&recv_poll, 1, 1000);
        std::cout << "Pre recv " << (int) recv_poll.revents << std::endl;
        if (!(recv_poll.revents & POLLIN)) {
            is_connected = false;
            std::cout << "[NETWORK BG] Cannot receive from socket" << std::endl;
            close(client_fd);
            connectUser();
        } Unnecessary - this causes an error when waiting for a recv for over a second */
        int cur_recv_len = recv(client_fd, tmp_buffer, cur_max_recv, 0);
        if (cur_recv_len == -1) {
            if (errno != EAGAIN) {
                is_connected = false;
                std::cout << "[NETWORK BG] Error while receiving:" << std::endl;
                perror("recv");
                close(client_fd);
                connectUser();
            }
            return -1;
        }
        else if (cur_recv_len == 0) {
            is_connected = false;
            std::cout << "[NETWORK BG] Socket closed" << std::endl;
            close(client_fd);
            connectUser();
            return -1;
        }
        // Update the received data counter
        recv_len += cur_recv_len;
    }
    return recv_len;
}



#include "network_bg_proc_obj.hpp"
#include "shared_mem.hpp"
#include "ardpiwire.hpp"

const char CV = 0;
const char VAR = 1;

void senderThread(NetworkSharedObj* volatile shared_obj, WirelessConnection* conn) {
    while (true) {
        if (!shared_obj->mutex.try_lock()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (shared_obj->send_flag_cv) {
            std::cout << "Sending CV" << std::endl;
            conn->sendData(&CV, 1);
            conn->sendData(shared_obj->data_cv, BUFFER_SIZE_CV);
            shared_obj->send_flag_cv = false;
            std::cout << "Sent CV" << std::endl;
        }

        if (shared_obj->send_flag_var) {
            conn->sendData(&VAR, 1);
            conn->sendData(&shared_obj->index_var, 1);
            conn->sendData((char*) &shared_obj->data_var, 4);
            shared_obj->send_flag_var = false;
        }

        shared_obj->mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

enum RecvInstructions : char {
    ARDUINO_SET_VAR,
    ARDUINO_GET_VAR,
    PI_SET_DIR,
    PI_SET_DRIVE_MODE
};
enum PiDirs : char {
    N, F, FR, R, BR, B, BL, L, FL
};
enum PiDriveModes : char {
    CAMERA, REMOTE
};

void recverThread(NetworkSharedObj* volatile shared_obj, WirelessConnection* conn) {
    PiDirs cur_remote_dir;
    PiDriveModes cur_drive_mode;
    char cur_dir, prev_dir;
    while (true) {
        char ins;
        if (conn->recvData(&ins, 1) != 1) continue;

        if (ins == ARDUINO_SET_VAR) {
            char targ_ind;
            if (conn->recvData(&targ_ind, 1) != 1) continue;
            float targ_val;
            if (conn->recvData((char*) &targ_val, 1) != 4) continue;

            ArdPiWire::sendByte(0);         // Send ins
            ArdPiWire::sendByte(targ_ind);  // Index
            ArdPiWire::sendFloat(targ_val); // Val
        }
        else if (ins == ARDUINO_GET_VAR) {
            char targ_ind;
            if (conn->recvData(&targ_ind, 1) != 1) continue;

            ArdPiWire::sendByte(1);         // Get ins
            ArdPiWire::sendByte(targ_ind);  // Index
            float recv = ArdPiWire::recvFloat(true);

            shared_obj->mutex.lock();
            shared_obj->index_var = targ_ind;
            shared_obj->data_var = recv;
            shared_obj->send_flag_var = true;
            shared_obj->mutex.unlock();
        }
        else if (ins == PI_SET_DIR) {
            char dir;
            if (conn->recvData(&dir, 1) != 1) continue;
            cur_remote_dir = (PiDirs) dir;
        }
        else if (ins == PI_SET_DRIVE_MODE) {
            char mode;
            if (conn->recvData(&mode, 1) != 1) continue;
            cur_remote_dir = (PiDirs) mode;
        }
        
        if (cur_drive_mode == CAMERA) {
            shared_obj->mutex.lock();
            char camera_dir = shared_obj->dir;
            shared_obj->mutex.unlock();

            switch (camera_dir) {
            case 0b010: cur_dir = PiDirs::F; break;
            case 0b100: cur_dir = PiDirs::L; break;
            case 0b001: cur_dir = PiDirs::R; break;
            }
        }
        else if (cur_drive_mode == REMOTE) cur_dir = cur_remote_dir;

        if (cur_dir != prev_dir) {
            float motor_l, motor_r;
            switch (cur_dir) {
            case PiDirs::N:     motor_l = 0;    motor_r = 0;    break;
            case PiDirs::F:     motor_l = 30;   motor_r = 30;   break;
            case PiDirs::FR:    motor_l = 30;   motor_r = 0;    break;
            case PiDirs::R:     motor_l = 30;   motor_r = -30;  break;
            case PiDirs::BR:    motor_l = -30;  motor_r = 0;    break;
            case PiDirs::B:     motor_l = -30;  motor_r = -30;  break;
            case PiDirs::BL:    motor_l = 0;    motor_r = -30;  break;
            case PiDirs::L:     motor_l = -30;  motor_r = 30;   break;
            case PiDirs::FL:    motor_l = 0;    motor_r = 30;   break;
            }
            ArdPiWire::sendByte(0);         // Send ins
            ArdPiWire::sendByte(11);        // L motor speed
            ArdPiWire::sendFloat(motor_l);
            ArdPiWire::sendByte(0);         // Send ins
            ArdPiWire::sendByte(12);        // R motor speed
            ArdPiWire::sendFloat(motor_r);
        }
        prev_dir = cur_dir;

        shared_obj->mutex.unlock();
    }
}

int main(int argc, char* argv[]) {
    
    ArdPiWire::init();
    SharedMemory shared_mem_host(sizeof(NetworkSharedObj), true);
    NetworkSharedObj* shared_obj = (NetworkSharedObj *) shared_mem_host.ptr;
    WirelessConnection conn;

    shared_obj->mutex.try_lock();
    shared_obj->mutex.unlock();
    shared_obj->send_flag_cv = false;
    shared_obj->send_flag_var = false;

    std::thread sender_thread(senderThread, shared_obj, &conn);
    std::thread recver_thread(recverThread, shared_obj, &conn);

    sender_thread.join();
    recver_thread.join();

    return 0;
}