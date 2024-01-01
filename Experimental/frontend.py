import os
import time
import requests
import cv2
import numpy as np
import socket


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


HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 12345  # The port used by the server
url = get_env_variable("IMG_HOST", False) # URL used by phone camera
login = get_env_variable("IMG_LOGIN") # login passed to phone camera url
password = get_env_variable("IMG_PASSWORD") # password passed to phone camera url
CLOSE_MESSAGE_SIZE=7312
VIDEO_X=720
VIDEO_Y=720
os.system("start target/FruitDetectionService.exe") #starting backend Service
time.sleep(1) #wait for backend service to propely start

#connect to backend
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


def SendImageToProcess(imageToSend: np.ndarray):
    size=imageToSend.size
    raw=int.to_bytes(size,byteorder='little',length=4)
    s.send(raw)
    raw=imageToSend
    s.send(raw)


def GetProcessedImage():
    raw=s.recv(4)
    size=int.from_bytes(raw,byteorder='little')
    currentData=np.array(bytearray(), dtype=np.uint8)
    while True:
        raw=s.recv(size)
        data=raw
        sampleData=np.array(bytearray(data), dtype=np.uint8)
        currentData=np.concatenate((currentData,sampleData))
        if(currentData.size>=VIDEO_X*VIDEO_Y*3):
            break
    return currentData

def CloseServer():
    size=CLOSE_MESSAGE_SIZE
    raw=int.to_bytes(size,byteorder='little',length=4)
    s.send(raw)




while True:
    img_resp = request_image(url, login, password)
    img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8)
    img = cv2.imdecode(img_arr, -1)
    SendImageToProcess(img)
    processed_img=GetProcessedImage()
    display_img=processed_img.reshape(VIDEO_Y,VIDEO_X,3)
    cv2.imshow("Fruit Detection", display_img)

    # Press Esc key to exit
    if cv2.waitKey(1) == 27:
        cv2.destroyAllWindows()
        CloseServer()
        time.sleep(1)
        s.close()
        exit(0)

