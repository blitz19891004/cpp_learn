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
#endif

void networkByteOrderDemo() {
    uint16_t hostPort = 13400;
    uint16_t networkPort = htons(hostPort);
    uint16_t backToHost = ntohs(networkPort);
    
    std::cout << "主机字节序端口: " << hostPort << std::endl;
    std::cout << "网络字节序端口: " << networkPort << std::endl;
    std::cout << "转换回主机字节序: " << backToHost << std::endl;
}

void ipAddressDemo() {
    const char* ipStr = "192.168.1.100";
    struct in_addr addr;
    
    if (inet_pton(AF_INET, ipStr, &addr) == 1) {
        std::cout << "IP字符串: " << ipStr << std::endl;
        std::cout << "转换为网络地址成功" << std::endl;
        
        char ipBuffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr, ipBuffer, INET_ADDRSTRLEN);
        std::cout << "转换回字符串: " << ipBuffer << std::endl;
    }
}

void socketTypeDemo() {
    std::cout << "Socket类型说明:" << std::endl;
    std::cout << "SOCK_STREAM: TCP协议，可靠连接" << std::endl;
    std::cout << "SOCK_DGRAM: UDP协议，无连接" << std::endl;
    std::cout << "AF_INET: IPv4地址族" << std::endl;
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup失败" << std::endl;
        return 1;
    }
#endif

    std::cout << "=== 第3课：TCP网络编程基础 ===" << std::endl;
    
    std::cout << "\n1. 网络字节序转换:" << std::endl;
    networkByteOrderDemo();
    
    std::cout << "\n2. IP地址转换:" << std::endl;
    ipAddressDemo();
    
    std::cout << "\n3. Socket类型:" << std::endl;
    socketTypeDemo();
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}

