#include <iostream>
#include <vector>
#include <iomanip>

class UDSMessage {
public:
    enum ServiceID {
        TesterPresent = 0x3E,
        ReadDataByIdentifier = 0x22,
        WriteDataByIdentifier = 0x2E,
        ReadMemoryByAddress = 0x23,
        WriteMemoryByAddress = 0x3D,
        RequestDownload = 0x34,
        RequestUpload = 0x35,
        TransferData = 0x36,
        RequestTransferExit = 0x37,
        SecurityAccess = 0x27,
        SessionControl = 0x10,
        Reset = 0x11,
        ClearDiagnosticInformation = 0x14
    };
    
    enum ResponseCode {
        PositiveResponse = 0x40,
        NegativeResponse = 0x7F
    };
    
    enum NegativeResponseCode {
        GeneralReject = 0x10,
        ServiceNotSupported = 0x11,
        SubFunctionNotSupported = 0x12,
        IncorrectMessageLength = 0x13,
        ResponseTooLong = 0x14,
        ConditionsNotCorrect = 0x22,
        RequestSequenceError = 0x24,
        NoResponseFromSubnetComponent = 0x25,
        FailurePreventsExecution = 0x26,
        RequestOutOfRange = 0x31,
        SecurityAccessDenied = 0x33,
        InvalidKey = 0x35,
        ExceedNumberOfAttempts = 0x36,
        RequiredTimeDelayNotExpired = 0x37
    };
    
    static std::vector<uint8_t> createSessionControl(uint8_t sessionType) {
        std::vector<uint8_t> message;
        message.push_back(SessionControl);
        message.push_back(sessionType);
        return message;
    }
    
    static std::vector<uint8_t> createTesterPresent() {
        std::vector<uint8_t> message;
        message.push_back(TesterPresent);
        message.push_back(0x00);
        return message;
    }
    
    static std::vector<uint8_t> createReadDataByIdentifier(uint16_t did) {
        std::vector<uint8_t> message;
        message.push_back(ReadDataByIdentifier);
        message.push_back((did >> 8) & 0xFF);
        message.push_back(did & 0xFF);
        return message;
    }
    
    static std::vector<uint8_t> createSecurityAccess(uint8_t level) {
        std::vector<uint8_t> message;
        message.push_back(SecurityAccess);
        message.push_back(level);
        return message;
    }
    
    static std::vector<uint8_t> createRequestDownload(uint32_t address, uint32_t length) {
        std::vector<uint8_t> message;
        message.push_back(RequestDownload);
        message.push_back(0x44);
        message.push_back((address >> 24) & 0xFF);
        message.push_back((address >> 16) & 0xFF);
        message.push_back((address >> 8) & 0xFF);
        message.push_back(address & 0xFF);
        message.push_back((length >> 24) & 0xFF);
        message.push_back((length >> 16) & 0xFF);
        message.push_back((length >> 8) & 0xFF);
        message.push_back(length & 0xFF);
        return message;
    }
    
    static std::vector<uint8_t> createTransferData(uint8_t blockSequence, const std::vector<uint8_t>& data) {
        std::vector<uint8_t> message;
        message.push_back(TransferData);
        message.push_back(blockSequence);
        message.insert(message.end(), data.begin(), data.end());
        return message;
    }
    
    static std::vector<uint8_t> createRequestTransferExit() {
        std::vector<uint8_t> message;
        message.push_back(RequestTransferExit);
        message.push_back(0x00);
        return message;
    }
    
    static void parseResponse(const std::vector<uint8_t>& data) {
        if (data.empty()) {
            std::cout << "响应为空" << std::endl;
            return;
        }
        
        if (data[0] >= PositiveResponse && data[0] < 0x80) {
            uint8_t serviceID = data[0] - PositiveResponse;
            std::cout << "正响应，服务ID: 0x" << std::hex << static_cast<int>(serviceID) << std::endl;
            
            if (data.size() > 1) {
                std::cout << "响应数据: ";
                for (size_t i = 1; i < data.size(); i++) {
                    std::cout << std::hex << std::setw(2) << std::setfill('0') 
                             << static_cast<int>(data[i]) << " ";
                }
                std::cout << std::endl;
            }
        } else if (data[0] == NegativeResponse) {
            if (data.size() >= 3) {
                uint8_t serviceID = data[1];
                uint8_t nrc = data[2];
                std::cout << "负响应" << std::endl;
                std::cout << "  服务ID: 0x" << std::hex << static_cast<int>(serviceID) << std::endl;
                std::cout << "  错误码: 0x" << std::hex << static_cast<int>(nrc) << std::endl;
            }
        } else {
            std::cout << "未知响应格式" << std::endl;
        }
    }
    
    static void printHex(const std::vector<uint8_t>& data) {
        std::cout << "UDS消息: ";
        for (uint8_t byte : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
    }
};

int main() {
    std::cout << "=== 第7课：UDS协议基础 ===" << std::endl;
    
    std::cout << "\n1. 会话控制 (默认会话):" << std::endl;
    auto msg1 = UDSMessage::createSessionControl(0x01);
    UDSMessage::printHex(msg1);
    
    std::cout << "\n2. TesterPresent:" << std::endl;
    auto msg2 = UDSMessage::createTesterPresent();
    UDSMessage::printHex(msg2);
    
    std::cout << "\n3. 读取数据标识符 (DID 0xF190):" << std::endl;
    auto msg3 = UDSMessage::createReadDataByIdentifier(0xF190);
    UDSMessage::printHex(msg3);
    
    std::cout << "\n4. 安全访问 (请求种子):" << std::endl;
    auto msg4 = UDSMessage::createSecurityAccess(0x01);
    UDSMessage::printHex(msg4);
    
    std::cout << "\n5. 请求下载 (地址0x08000000, 长度1024):" << std::endl;
    auto msg5 = UDSMessage::createRequestDownload(0x08000000, 1024);
    UDSMessage::printHex(msg5);
    
    std::cout << "\n6. 传输数据 (块序号1):" << std::endl;
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
    auto msg6 = UDSMessage::createTransferData(1, data);
    UDSMessage::printHex(msg6);
    
    std::cout << "\n7. 解析正响应示例:" << std::endl;
    std::vector<uint8_t> posResponse = {0x62, 0xF1, 0x90, 0x12, 0x34};
    UDSMessage::parseResponse(posResponse);
    
    std::cout << "\n8. 解析负响应示例:" << std::endl;
    std::vector<uint8_t> negResponse = {0x7F, 0x22, 0x31};
    UDSMessage::parseResponse(negResponse);
    
    return 0;
}

