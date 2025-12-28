# C++学习指南 - DoIP/UDS协议开发

## 快速开始

### 编译和运行单个课程

```bash
cd lesson01_basics
mkdir -p build
cd build
cmake ..
cmake --build .
./lesson01    # Linux/Mac
lesson01.exe  # Windows
```

### 课程列表

1. **lesson01_basics** - C++基础语法
2. **lesson02_stl** - STL容器和字符串处理
3. **lesson03_tcp_basic** - TCP网络编程基础
4. **lesson04_tcp_server** - TCP Server实现
5. **lesson05_byte_stream** - 字节流处理
6. **lesson06_doip** - DoIP协议基础
7. **lesson07_uds** - UDS协议基础
8. **lesson08_doip_flash** - DoIP刷写实现

## 测试TCP Server

### 第4课和第8课需要Python客户端测试

创建 `test_client.py`:

```python
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 13400))
s.send(b'Hello from Python')
response = s.recv(1024)
print(f"收到响应: {response}")
s.close()
```

运行:
```bash
python test_client.py
```

## 学习顺序

严格按照课程顺序学习，每课30分钟：
1. 阅读代码理解概念
2. 编译运行查看输出
3. 尝试修改代码加深理解
4. 进入下一课

## 注意事项

- Windows系统需要安装Visual Studio或MinGW
- Linux/Mac需要安装g++和cmake
- 网络相关课程需要管理员权限（端口绑定）

