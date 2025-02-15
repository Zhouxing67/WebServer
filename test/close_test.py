import socket
import threading


def connect_and_close():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("localhost", 8888))
        s.close()
    except Exception as e:
        pass  # 忽略连接错误


Threads = 100000
# 创建并启动100个线程
threads = [threading.Thread(target=connect_and_close) for _ in range(Threads)]
for t in threads:
    t.start()
for t in threads:
    t.join()
