import numpy as np
import cv2 as cv
import datetime
from helper_functions import helper_functions

date = datetime.datetime.now().strftime("%y%m%d")
img = np.zeros((512, 512, 3), np.uint8)

# Draw line - diagonal blue
cv.line(img, (0, 0), (511, 511), (255, 0, 0), 5)

# Drawing Rectangle - top left green
cv.rectangle(img, (384, 0), (510, 128), (0, 255, 0), 3)

# Drawing Circle - inside rectangle
cv.circle(img, (447, 63), 63, (0, 0, 255), -1)

# Drawing ellipse - half ellipse
cv.ellipse(img, (256, 256), (100, 50), 0, 0, 180, 255, -1)

# Drawing Polygon
pts = np.array([[10, 5], [20, 30], [70, 20], [50, 10]], np.int32)
pts = pts.reshape((-1, 1, 2))
cv.polylines(img, [pts], True, (0,255,255))

# Drawing text
font = cv.FONT_HERSHEY_SIMPLEX
cv.putText(img, "OpenCV", (10,500), font, 4, (255,255,255),2,cv.LINE_AA)

cv.imshow('img', img)

k = cv.waitKey(0)

if k == ord("s"):
    ret = image_wr_helper.save_image(img, "linesquarecirclepolywords", "cvlines")
    if ret != 0:
        assert "File not saved"
if k == ord("q"):
    exit()