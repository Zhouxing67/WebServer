import socket
import threading
import time
import traceback
import sys


def connect_and_close():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("localhost", 8888))
        s.send(b"HELLO")
        response = s.recv(32)
        print(
            f"Received:{response.decode()}",
        )
        time.sleep(5)  # 等待10s,模拟业务处理
        s.close()
    except Exception:
        print("连接或通信失败！")
        traceback.print_exc()  # 打印失败原因


def start():
    if len(sys.argv) > 1:
        concurrent = int(sys.argv[1])
    else:
        concurrent = 80000
    threads = [threading.Thread(target=connect_and_close)
               for _ in range(concurrent)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()


start()
