import numpy as np
import cv2 as cv
import datetime
from helper_functions import image_wr_helper

date = datetime.datetime.now().strftime("%y%m%d")
img = np.zeros((512, 512, 3), np.uint8)

cv.line(img, (0, 0), (511, 511), (255, 0, 0), 5)
cv.imshow('img', img)

k = cv.waitKey(0)

if k == ord("s"):
    ret = image_wr_helper.save_image(img, "blueline", "cvlines")
    if ret != 0:
        assert "File not saved"
