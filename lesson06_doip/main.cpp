#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>

class DoIPMessage {
private:
    std::vector<uint8_t> payload;
    
public:
    static const uint8_t PROTOCOL_VERSION = 0x02;
    static const uint8_t INVERSE_VERSION = 0xFD;
    
    enum MessageType {
        VEHICLE_IDENTIFICATION_REQUEST = 0x0001,
        VEHICLE_IDENTIFICATION_RESPONSE = 0x0004,
        ROUTING_ACTIVATION_REQUEST = 0x0005,
        ROUTING_ACTIVATION_RESPONSE = 0x0006,
        DIAGNOSTIC_MESSAGE = 0x8001,
        DIAGNOSTIC_MESSAGE_ACK = 0x8002,
        DIAGNOSTIC_MESSAGE_NACK = 0x8003
    };
    
    static std::vector<uint8_t> createVehicleIdentificationRequest() {
        std::vector<uint8_t> message;
        message.push_back(PROTOCOL_VERSION);
        message.push_back(INVERSE_VERSION);
        message.push_back((VEHICLE_IDENTIFICATION_REQUEST >> 8) & 0xFF);
        message.push_back(VEHICLE_IDENTIFICATION_REQUEST & 0xFF);
        message.push_back(0x00);
        message.push_back(0x00);
        message.push_back(0x00);
        message.push_back(0x00);
        return message;
    }
    
    static std::vector<uint8_t> createRoutingActivationRequest(uint16_t sourceAddress) {
        std::vector<uint8_t> message;
        message.push_back(PROTOCOL_VERSION);
        message.push_back(INVERSE_VERSION);
        message.push_back((ROUTING_ACTIVATION_REQUEST >> 8) & 0xFF);
        message.push_back(ROUTING_ACTIVATION_REQUEST & 0xFF);
        message.push_back(0x00);
        message.push_back(0x00);
        message.push_back(0x00);
        message.push_back(0x05);
        message.push_back((sourceAddress >> 8) & 0xFF);
        message.push_back(sourceAddress & 0xFF);
        message.push_back(0x00);
        message.push_back(0x00);
        message.push_back(0x00);
        return message;
    }
    
    static std::vector<uint8_t> createDiagnosticMessage(uint16_t sourceAddress, 
                                                         uint16_t targetAddress,
                                                         const std::vector<uint8_t>& udsData) {
        std::vector<uint8_t> message;
        message.push_back(PROTOCOL_VERSION);
        message.push_back(INVERSE_VERSION);
        message.push_back((DIAGNOSTIC_MESSAGE >> 8) & 0xFF);
        message.push_back(DIAGNOSTIC_MESSAGE & 0xFF);
        
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
    
    static void parseMessage(const std::vector<uint8_t>& data) {
        if (data.size() < 8) {
            std::cout << "消息太短" << std::endl;
            return;
        }
        
        uint8_t protocolVersion = data[0];
        uint8_t inverseVersion = data[1];
        uint16_t messageType = (data[2] << 8) | data[3];
        uint32_t payloadLength = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
        
        std::cout << "协议版本: 0x" << std::hex << static_cast<int>(protocolVersion) << std::endl;
        std::cout << "消息类型: 0x" << std::hex << messageType << std::endl;
        std::cout << "负载长度: " << std::dec << payloadLength << " 字节" << std::endl;
        
        if (messageType == DIAGNOSTIC_MESSAGE && data.size() >= 12) {
            uint16_t sourceAddr = (data[8] << 8) | data[9];
            uint16_t targetAddr = (data[10] << 8) | data[11];
            std::cout << "源地址: 0x" << std::hex << sourceAddr << std::endl;
            std::cout << "目标地址: 0x" << std::hex << targetAddr << std::endl;
            
            if (data.size() > 12) {
                std::cout << "UDS数据: ";
                for (size_t i = 12; i < data.size(); i++) {
                    std::cout << std::hex << std::setw(2) << std::setfill('0') 
                             << static_cast<int>(data[i]) << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    
    static void printHex(const std::vector<uint8_t>& data) {
        std::cout << "HEX: ";
        for (uint8_t byte : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
    }
};

int main() {
    std::cout << "=== 第6课：DoIP协议基础 ===" << std::endl;
    
    std::cout << "\n1. 车辆识别请求:" << std::endl;
    auto msg1 = DoIPMessage::createVehicleIdentificationRequest();
    DoIPMessage::printHex(msg1);
    DoIPMessage::parseMessage(msg1);
    
    std::cout << "\n2. 路由激活请求:" << std::endl;
    auto msg2 = DoIPMessage::createRoutingActivationRequest(0x0E00);
    DoIPMessage::printHex(msg2);
    DoIPMessage::parseMessage(msg2);
    
    std::cout << "\n3. 诊断消息:" << std::endl;
    std::vector<uint8_t> udsData = {0x10, 0x03};
    auto msg3 = DoIPMessage::createDiagnosticMessage(0x0E00, 0x0E80, udsData);
    DoIPMessage::printHex(msg3);
    DoIPMessage::parseMessage(msg3);
    
    return 0;
}

