import socket
import threading
import time
import traceback


def connect_and_close():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("localhost", 8888))
        time.sleep(10)  # 等待10s,模拟业务处理
        s.close()
    except Exception:
        print("连接或通信失败！")
        traceback.print_exc()  # 打印失败原因


Threads = 1
# 创建并启动100个线程
threads = [threading.Thread(target=connect_and_close) for _ in range(Threads)]
for t in threads:
    t.start()
for t in threads:
    t.join()
