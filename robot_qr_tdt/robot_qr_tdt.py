import cv2
import numpy as np
# import gspread
import imutils
import pyautogui
import os
from pyzbar.pyzbar import decode
# from oauth2client.service_account import ServiceAccountCredentials

# scope = ["https://spreadsheets.google.com/feeds", 'https://www.googleapis.com/auth/spreadsheets',
#          "https://www.googleapis.com/auth/drive.file", "https://www.googleapis.com/auth/drive"]

# creds = ServiceAccountCredentials.from_json_keyfile_name(
#     'googlesheet.json', scope)
# client = gspread.authorize(creds)
# sheet = client.open("data_robot").sheet1


# cap = cv2.VideoCapture(0)
# cap.set(3, 640)
# cap.set(4, 480)

# with open('myDataFile.text') as f:
#     myDataList = f.read().splitlines()
path = 'screenshot.png'

while True:
    pyautogui.screenshot("screenshot.png")
    # success, img = cap.read()
    img = cv2.imread("screenshot.png")
    for barcode in decode(img):
        myData = barcode.data.decode('utf-8')
        print(myData)
        pts = np.array([barcode.polygon], np.int32)
        pts = pts.reshape((-1, 1, 2))
        cv2.polylines(img, [pts], True, (255, 0, 255), 5)
        pts2 = barcode.rect
        cv2.putText(img, myData, (pts2[0], pts2[1]),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.9, (255, 0, 255), 2)
        # sheet.append_row([myData])
    # cv2.putText(img, myData, (pts2 & [0], pts2[1]),cv2.FONT_HERSHEY_SIMPLEX, 0.9, myColor, 2)
    os.remove(path)

    cv2.imshow('Result', imutils.resize(img, width=600))

    cv2.waitKey(1)
