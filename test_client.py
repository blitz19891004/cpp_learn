import socket
import time

def test_tcp_server():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('127.0.0.1', 13400))
        print("连接成功!")
        
        message = b'Hello from Python client'
        s.send(message)
        print(f"发送: {message}")
        
        response = s.recv(1024)
        print(f"收到响应: {response}")
        
        s.close()
        print("连接已关闭")
    except ConnectionRefusedError:
        print("连接被拒绝，请确保服务器正在运行")
    except Exception as e:
        print(f"错误: {e}")

if __name__ == "__main__":
    test_tcp_server()

