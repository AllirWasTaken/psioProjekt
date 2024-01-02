import numpy as np

import cv2
import imutils
import os
import requests
import sys
import time


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


device=0
FRAME_WIDTH = 1280
FRAME_HEIGHT = 720
FPS=30
url = get_env_variable("IMG_HOST", False)
login = get_env_variable("IMG_LOGIN")
password = get_env_variable("IMG_PASSWORD")

cap = cv2.VideoCapture(device)
cap.set(cv2.CAP_PROP_FRAME_WIDTH,FRAME_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT)
cap.set(cv2.CAP_PROP_FPS,FPS)

print(cap.isOpened())

pos=0
first=True
run = True

while run:
    t = time.time()
    for i in range(0, 30):
        img_resp = request_image(url, login, password)
        img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8)
        img = cv2.imdecode(img_arr, -1)
        edges = cv2.Canny(img, 100, 200)
        #img = imutils.resize(img, width=1000, height=1000)
        #cv2.imshow("Android_cam", img)
        cv2.imshow("Edges", edges)

        # Press Esc key to exit
        if cv2.waitKey(1) == 27:
            run = False
    t = time.time() - t
    print("FPS", 30 / t)
cv2.destroyAllWindows()



