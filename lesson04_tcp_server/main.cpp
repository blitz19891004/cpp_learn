#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

class TCPServer {
private:
    SOCKET serverSocket;
    int port;
    
public:
    TCPServer(int p) : port(p), serverSocket(INVALID_SOCKET) {}
    
    bool start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "创建socket失败" << std::endl;
            return false;
        }
        
        int opt = 1;
#ifdef _WIN32
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
        
        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "绑定端口失败" << std::endl;
            closesocket(serverSocket);
            return false;
        }
        
        if (listen(serverSocket, 5) == SOCKET_ERROR) {
            std::cerr << "监听失败" << std::endl;
            closesocket(serverSocket);
            return false;
        }
        
        std::cout << "服务器启动成功，监听端口: " << port << std::endl;
        return true;
    }
    
    void acceptClient() {
        sockaddr_in clientAddr;
#ifdef _WIN32
        int addrLen = sizeof(clientAddr);
#else
        socklen_t addrLen = sizeof(clientAddr);
#endif
        
        std::cout << "等待客户端连接..." << std::endl;
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
        
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "接受连接失败" << std::endl;
            return;
        }
        
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "客户端连接: " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
        
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "收到数据: " << buffer << std::endl;
            
            const char* response = "Hello from server!";
            send(clientSocket, response, strlen(response), 0);
        }
        
        closesocket(clientSocket);
    }
    
    void stop() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }
    }
    
    ~TCPServer() {
        stop();
    }
};

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup失败" << std::endl;
        return 1;
    }
#endif

    std::cout << "=== 第4课：TCP Server实现 ===" << std::endl;
    
    TCPServer server(13400);
    
    if (!server.start()) {
        std::cerr << "服务器启动失败" << std::endl;
        return 1;
    }
    
    std::cout << "\n提示: 使用Python客户端连接测试:" << std::endl;
    std::cout << "import socket" << std::endl;
    std::cout << "s = socket.socket()" << std::endl;
    std::cout << "s.connect(('127.0.0.1', 13400))" << std::endl;
    std::cout << "s.send(b'Hello from Python')" << std::endl;
    std::cout << "print(s.recv(1024))" << std::endl;
    std::cout << "\n按Ctrl+C退出服务器" << std::endl;
    
    while (true) {
        server.acceptClient();
    }
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}

