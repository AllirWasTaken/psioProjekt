import sys
import time
import requests
import cv2
import numpy as np
import socket
import subprocess  

subprocess.run(["target\FruitDetectionService.exe"]) #starting backend Service

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 65432  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"Hello, world")
    data = s.recv(1024)

print(f"Received {data!r}")

FPS=30
url = "http://192.168.43.122:8080/shot.jpg"

pos=0
first=True

while True:
    t = time.time()
    for i in range(0, 30):
        img_resp = requests.get(url)
        img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8)
        img = cv2.imdecode(img_arr, -1)
        cv2.imshow("Android_cam", img)

        # Press Esc key to exit
        if cv2.waitKey(1) == 27:
            exit(0)
    t = time.time() - t
    print("FPS", 30 / t)
cv2.destroyAllWindows()