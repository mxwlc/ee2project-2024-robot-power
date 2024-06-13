#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <iostream>
#include <unistd.h>
#include <mutex>

#define PORT 9876
#define MAX_RECV_BUFFER_SIZE 8192
#define MAX_SEND_BUFFER_SIZE MAX_RECV_BUFFER_SIZE

class WirelessConnection {

    int listener_fd, client_fd;
    sockaddr_in client;
    std::mutex mutex;

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
        perror("setsockopt(... SO_RCV_TIMEO...)");
        exit(1);
    }

    std::cout << "[NETWORK BG] Connected new client" << std::endl;
}

WirelessConnection::WirelessConnection() {
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
    bind(listener_fd, (sockaddr *) &addr, sizeof(addr));

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

        while (send(client_fd, data, cur_send_len, 0) == -1) {
            std::cout << "[NETWORK BG] Error while sending:" << std::endl;
            perror("send");
            std::this_thread::sleep_for(std::chrono::seconds(1));
            //close(client_fd);
            //connectUser();
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

        int cur_recv_len = recv(client_fd, tmp_buffer, cur_max_recv, 0);
        if (cur_recv_len == -1) {
            if (errno != EAGAIN) {
                std::cout << "[NETWORK BG] Error while receiving:" << std::endl;
                perror("recv");
                close(client_fd);
                connectUser();    
            }
            return -1;
        }
        // Update the received data counter
        recv_len += cur_recv_len;
    }
    return recv_len;
}



#include "network_bg_proc_obj.hpp"
#include "shared_mem.hpp"
#include "ardpiwire.cpp"

#include <chrono>
#include <thread>

const char CV = 0;

void senderThread(NetworkSharedObj* shared_obj, WirelessConnection* conn) {
    while (true) {
        shared_obj->mutex.lock();

        if (shared_obj->send_flag_cv) {
            conn->sendData(&CV, 1);
            conn->sendData(shared_obj->data_cv, BUFFER_SIZE_CV);
            shared_obj->send_flag_cv = false;
        }

        shared_obj->mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void recverThread(WirelessConnection* conn) {
    while (true) {
        char buffer;
        float buffer2;
        if (conn->recvData(&buffer, 1) != 1) continue;
        ArdPiWire::sendByte(buffer);
    }
}

int main(int argc, char* argv[]) {
    
    SharedMemory shared_mem_host(sizeof(NetworkSharedObj), true);
    NetworkSharedObj* shared_obj = (NetworkSharedObj *) shared_mem_host.ptr;
    WirelessConnection conn;
    ArdPiWire::init();

    shared_obj->mutex.try_lock();
    shared_obj->mutex.unlock();
    shared_obj->send_flag_cv = false;

    std::thread sender_thread(senderThread, shared_obj, &conn);
    std::thread recver_thread(recverThread, &conn);

    sender_thread.join();
    recver_thread.join();

    return 0;
}