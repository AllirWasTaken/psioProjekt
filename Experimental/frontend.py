import os
import time
import requests
import cv2
import numpy as np
import signal
import socket
import subprocess as sp


def get_env_variable(name, optional=True):
    variable = os.environ.get(name, None)
    if not variable and not optional:
        print(f'Environmental variable {name} not set')
        exit(-1)
    return variable


def request_image(url, login=None, password=None):
    if not login:
        return requests.get(url)
    return requests.get(url, auth=(login, password))


def read_pid(file):
    contents = file.read()
    return int(contents)

def write_pid(file, pid):
    file.truncate(0)
    file.write(str(pid))


HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 12345  # The port used by the server
url = get_env_variable("IMG_HOST", False) # URL used by phone camera
login = get_env_variable("IMG_LOGIN") # login passed to phone camera url
password = get_env_variable("IMG_PASSWORD") # password passed to phone camera url
CLOSE_MESSAGE_SIZE=7312
VIDEO_X=720
VIDEO_Y=720
SERVICE_PID_FILE="service.pid"

with open(SERVICE_PID_FILE, 'w+') as f:
    try:
        old_pid = read_pid(f)
        os.kill(old_pid, signal.SIGTERM) # kill service if it didn't close
    except:
        pass
    service = sp.Popen(["cmd", "/C", "target\FruitDetectionService.exe"],
                       creationflags=sp.CREATE_NEW_CONSOLE) #starting backend Service
    write_pid(f, service.pid)

time.sleep(1) #wait for backend service to propely start

#connect to backend
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


def SendImageToProcess(imageToSend: np.ndarray):
    size=imageToSend.size
    raw=int.to_bytes(size,byteorder='little',length=4)
    s.sendall(raw)
    raw=imageToSend
    s.sendall(raw)


def GetProcessedImage():
    raw=s.recv(4)
    size=int.from_bytes(raw,byteorder='little')
    buffer = bytearray()
    while size > 0:
        msg = s.recv(size)
        buffer.extend(msg)
        size -= len(msg)
    return np.array(buffer, dtype=np.uint8)

def CloseServer():
    size=CLOSE_MESSAGE_SIZE
    raw=int.to_bytes(size,byteorder='little',length=4)
    s.sendall(raw)


def crop_image(image, width, height):
    assert(len(image.shape) <= 3)
    y, x, *_ = image.shape
    assert(x >= width)
    assert(y >= height)
    center_x = x // 2
    center_y = y // 2
    half_w = width // 2
    half_h = height // 2
    new_image = image[center_y - half_h : center_y + half_h,
                      center_x - half_w : center_x + half_w]
    return np.ascontiguousarray(new_image)

while True:
    try:
        img_resp = request_image(url, login, password)
    except ConnectionError as e:
        print("Failed to receive image:", e)
        break
    if (img_resp.status_code != requests.codes.OK):
        print("Invalid response:", img_resp)
        break
    img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8)
    img = cv2.imdecode(img_arr, -1)
    img = crop_image(img, VIDEO_X, VIDEO_Y)

    SendImageToProcess(img)
    processed_img=GetProcessedImage()
    display_img=processed_img.reshape(VIDEO_Y,VIDEO_X,3)
    cv2.imshow("Fruit Detection", display_img)

    # Press Esc key to exit
    if cv2.waitKey(1) == 27:
        break
cv2.destroyAllWindows()
CloseServer()
time.sleep(1)
s.close()

