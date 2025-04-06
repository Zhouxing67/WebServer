import socket
import time

SERVER_IP = "127.0.0.1"
SERVER_PORT = 8888

for _ in range(10):  # 模拟 10 次连接
    try:
        # 创建一个 socket
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # 连接到服务器
        client_socket.connect((SERVER_IP, SERVER_PORT))
        # 主动关闭连接
        client_socket.shutdown(socket.SHUT_WR)
    except Exception as e:
        print(f"Error: {e}")
    finally:
        time.sleep(0.01)  # 避免过于频繁的连接导致错误
