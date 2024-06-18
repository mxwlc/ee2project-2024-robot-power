import cv2 as cv
import sys
import time

img = cv.imread(cv.samples.findFile("images/cat/cleo0304.jpg"))

if img is None:
    sys.exit("Could not find image")

cv.imshow("Display Image", img)
k = cv.waitKey(0)

if k == ord('s'):
    cv.imwrite("images/output.jpg", img)