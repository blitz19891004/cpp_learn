#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>

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

class DoIPFlashServer {
private:
    SOCKET serverSocket;
    int port;
    uint16_t sourceAddress;
    uint16_t targetAddress;
    bool sessionActive;
    bool securityUnlocked;
    
    std::vector<uint8_t> createDoIPDiagnosticMessage(const std::vector<uint8_t>& udsData) {
        std::vector<uint8_t> message;
        message.push_back(0x02);
        message.push_back(0xFD);
        message.push_back(0x80);
        message.push_back(0x01);
        
        uint32_t payloadLength = 4 + udsData.size();
        message.push_back((payloadLength >> 24) & 0xFF);
        message.push_back((payloadLength >> 16) & 0xFF);
        message.push_back((payloadLength >> 8) & 0xFF);
        message.push_back(payloadLength & 0xFF);
        
        message.push_back((sourceAddress >> 8) & 0xFF);
        message.push_back(sourceAddress & 0xFF);
        message.push_back((targetAddress >> 8) & 0xFF);
        message.push_back(targetAddress & 0xFF);
        
        message.insert(message.end(), udsData.begin(), udsData.end());
        return message;
    }
    
    std::vector<uint8_t> handleUDSRequest(const std::vector<uint8_t>& udsData) {
        if (udsData.empty()) {
            return {0x7F, 0x00, 0x13};
        }
        
        uint8_t serviceID = udsData[0];
        
        switch (serviceID) {
            case 0x10:
                if (udsData.size() >= 2 && udsData[1] == 0x03) {
                    sessionActive = true;
                    return {0x50, 0x03};
                }
                return {0x7F, 0x10, 0x22};
                
            case 0x27:
                if (udsData.size() >= 2) {
                    if (udsData[1] == 0x01) {
                        return {0x67, 0x01, 0x12, 0x34, 0x56, 0x78};
                    } else if (udsData[1] == 0x02) {
                        securityUnlocked = true;
                        return {0x67, 0x02};
                    }
                }
                return {0x7F, 0x27, 0x35};
                
            case 0x34:
                if (udsData.size() >= 10) {
                    uint32_t address = (udsData[2] << 24) | (udsData[3] << 16) | 
                                      (udsData[4] << 8) | udsData[5];
                    uint32_t length = (udsData[6] << 24) | (udsData[7] << 16) | 
                                     (udsData[8] << 8) | udsData[9];
                    std::cout << "  请求下载: 地址=0x" << std::hex << address 
                             << ", 长度=" << std::dec << length << std::endl;
                    return {0x74, 0x10, 0x00};
                }
                return {0x7F, 0x34, 0x13};
                
            case 0x36:
                if (udsData.size() >= 2) {
                    uint8_t blockSeq = udsData[1];
                    std::cout << "  传输数据块: " << static_cast<int>(blockSeq) << std::endl;
                    return {0x76, blockSeq};
                }
                return {0x7F, 0x36, 0x13};
                
            case 0x37:
                return {0x77, 0x00};
                
            case 0x3E:
                return {0x7E, 0x00};
                
            default:
                return {0x7F, serviceID, 0x11};
        }
    }
    
    void processClient(SOCKET clientSocket) {
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        
        if (bytesReceived > 0) {
            std::vector<uint8_t> receivedData(buffer, buffer + bytesReceived);
            
            std::cout << "\n收到DoIP消息 (" << bytesReceived << " 字节):" << std::endl;
            std::cout << "  HEX: ";
            for (int i = 0; i < bytesReceived; i++) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                         << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::dec << std::endl;
            
            if (bytesReceived >= 8) {
                uint16_t messageType = (buffer[2] << 8) | buffer[3];
                
                if (messageType == 0x8001) {
                    if (bytesReceived >= 12) {
                        std::vector<uint8_t> udsData(buffer + 12, buffer + bytesReceived);
                        std::cout << "  提取UDS数据: ";
                        for (uint8_t b : udsData) {
                            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                                     << static_cast<int>(b) << " ";
                        }
                        std::cout << std::dec << std::endl;
                        
                        std::vector<uint8_t> udsResponse = handleUDSRequest(udsData);
                        std::vector<uint8_t> doipResponse = createDoIPDiagnosticMessage(udsResponse);
                        
                        send(clientSocket, (char*)doipResponse.data(), doipResponse.size(), 0);
                        std::cout << "  发送响应" << std::endl;
                    }
                } else if (messageType == 0x0005) {
                    std::vector<uint8_t> response = {
                        0x02, 0xFD, 0x00, 0x06, 0x00, 0x00, 0x00, 0x05,
                        0x0E, 0x00, 0x00, 0x00, 0x00, 0x00
                    };
                    send(clientSocket, (char*)response.data(), response.size(), 0);
                    std::cout << "  路由激活响应" << std::endl;
                }
            }
        }
    }
    
public:
    DoIPFlashServer(int p, uint16_t src, uint16_t tgt) 
        : port(p), sourceAddress(src), targetAddress(tgt), 
          sessionActive(false), securityUnlocked(false), serverSocket(INVALID_SOCKET) {}
    
    bool start() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) return false;
        
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
            closesocket(serverSocket);
            return false;
        }
        
        if (listen(serverSocket, 5) == SOCKET_ERROR) {
            closesocket(serverSocket);
            return false;
        }
        
        std::cout << "DoIP刷写服务器启动，端口: " << port << std::endl;
        std::cout << "源地址: 0x" << std::hex << sourceAddress << std::endl;
        std::cout << "目标地址: 0x" << std::hex << targetAddress << std::dec << std::endl;
        return true;
    }
    
    void run() {
        while (true) {
            sockaddr_in clientAddr;
#ifdef _WIN32
            int addrLen = sizeof(clientAddr);
#else
            socklen_t addrLen = sizeof(clientAddr);
#endif
            
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
            if (clientSocket != INVALID_SOCKET) {
                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
                std::cout << "\n客户端连接: " << clientIP << std::endl;
                
                processClient(clientSocket);
                closesocket(clientSocket);
            }
        }
    }
    
    ~DoIPFlashServer() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
    }
};

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    std::cout << "=== 第8课：DoIP刷写实现 ===" << std::endl;
    std::cout << "\n支持的服务:" << std::endl;
    std::cout << "  0x10 - 会话控制" << std::endl;
    std::cout << "  0x27 - 安全访问" << std::endl;
    std::cout << "  0x34 - 请求下载" << std::endl;
    std::cout << "  0x36 - 传输数据" << std::endl;
    std::cout << "  0x37 - 请求传输退出" << std::endl;
    std::cout << "  0x3E - TesterPresent" << std::endl;
    
    DoIPFlashServer server(13400, 0x0E80, 0x0E00);
    
    if (!server.start()) {
        std::cerr << "服务器启动失败" << std::endl;
        return 1;
    }
    
    server.run();
    
#ifdef _WIN32
    WSACleanup();
#endif
    
    return 0;
}

