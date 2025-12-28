#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>

class ByteBuffer {
private:
    std::vector<uint8_t> data;
    size_t position;
    
public:
    ByteBuffer() : position(0) {}
    
    void writeByte(uint8_t value) {
        data.push_back(value);
    }
    
    void writeUint16(uint16_t value) {
        uint8_t bytes[2];
        bytes[0] = (value >> 8) & 0xFF;
        bytes[1] = value & 0xFF;
        data.push_back(bytes[0]);
        data.push_back(bytes[1]);
    }
    
    void writeUint32(uint32_t value) {
        uint8_t bytes[4];
        bytes[0] = (value >> 24) & 0xFF;
        bytes[1] = (value >> 16) & 0xFF;
        bytes[2] = (value >> 8) & 0xFF;
        bytes[3] = value & 0xFF;
        data.insert(data.end(), bytes, bytes + 4);
    }
    
    void writeBytes(const uint8_t* bytes, size_t len) {
        data.insert(data.end(), bytes, bytes + len);
    }
    
    uint8_t readByte() {
        if (position >= data.size()) return 0;
        return data[position++];
    }
    
    uint16_t readUint16() {
        if (position + 1 >= data.size()) return 0;
        uint16_t value = (data[position] << 8) | data[position + 1];
        position += 2;
        return value;
    }
    
    uint32_t readUint32() {
        if (position + 3 >= data.size()) return 0;
        uint32_t value = (data[position] << 24) | 
                        (data[position + 1] << 16) | 
                        (data[position + 2] << 8) | 
                        data[position + 3];
        position += 4;
        return value;
    }
    
    void reset() {
        position = 0;
    }
    
    const uint8_t* getData() const {
        return data.data();
    }
    
    size_t getSize() const {
        return data.size();
    }
    
    void printHex() const {
        std::cout << "数据(HEX): ";
        for (size_t i = 0; i < data.size(); i++) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                     << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << std::endl;
    }
};

void demoPack() {
    std::cout << "=== 打包数据 ===" << std::endl;
    ByteBuffer buffer;
    
    buffer.writeByte(0x10);
    buffer.writeUint16(0x1234);
    buffer.writeUint32(0x567890AB);
    
    uint8_t bytes[] = {0xAA, 0xBB, 0xCC};
    buffer.writeBytes(bytes, 3);
    
    buffer.printHex();
    std::cout << "总长度: " << buffer.getSize() << " 字节" << std::endl;
}

void demoUnpack() {
    std::cout << "\n=== 解包数据 ===" << std::endl;
    ByteBuffer buffer;
    
    buffer.writeByte(0x10);
    buffer.writeUint16(0x1234);
    buffer.writeUint32(0x567890AB);
    
    buffer.reset();
    
    uint8_t byte = buffer.readByte();
    uint16_t word = buffer.readUint16();
    uint32_t dword = buffer.readUint32();
    
    std::cout << "读取Byte: 0x" << std::hex << static_cast<int>(byte) << std::endl;
    std::cout << "读取Uint16: 0x" << std::hex << word << std::endl;
    std::cout << "读取Uint32: 0x" << std::hex << dword << std::endl;
}

void demoProtocolFrame() {
    std::cout << "\n=== 协议帧示例 ===" << std::endl;
    
    ByteBuffer frame;
    
    uint8_t header = 0x02;
    uint16_t length = 0x0005;
    uint8_t data[] = {0x10, 0x03, 0x00, 0x01, 0x02};
    uint8_t checksum = 0;
    
    frame.writeByte(header);
    frame.writeUint16(length);
    frame.writeBytes(data, 5);
    
    for (size_t i = 0; i < frame.getSize(); i++) {
        checksum ^= frame.getData()[i];
    }
    frame.writeByte(checksum);
    
    std::cout << "协议帧结构:" << std::endl;
    std::cout << "  Header: 0x02" << std::endl;
    std::cout << "  Length: 0x0005" << std::endl;
    std::cout << "  Data: 5字节" << std::endl;
    std::cout << "  Checksum: 0x" << std::hex << static_cast<int>(checksum) << std::endl;
    frame.printHex();
}

int main() {
    std::cout << "=== 第5课：字节流处理和协议解析 ===" << std::endl;
    
    demoPack();
    demoUnpack();
    demoProtocolFrame();
    
    return 0;
}

