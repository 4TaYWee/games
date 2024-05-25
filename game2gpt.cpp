
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <algorithm>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

mutex mtx;
vector<SOCKET> clients;

void handleClient(SOCKET clientSocket) {
    mtx.lock();
    clients.push_back(clientSocket);
    mtx.unlock();

    char buffer[256];
    while (true) {
        int n = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            mtx.lock();
            auto it = find(clients.begin(), clients.end(), clientSocket);
            if (it != clients.end()) {
                clients.erase(it);
            }
            mtx.unlock();
            closesocket(clientSocket);
            break;
        }

        mtx.lock();
        for (size_t i = 0; i < clients.size(); ++i) {
            if (clients[i] != clientSocket) {
                send(clients[i], buffer, n, 0);
            }
        }
        mtx.unlock();
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating socket" << endl;
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888);

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error binding socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Error listening on socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        SOCKADDR_IN clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Error accepting client connection" << endl;
            continue;
        }

        thread t(handleClient, clientSocket);
        t.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
