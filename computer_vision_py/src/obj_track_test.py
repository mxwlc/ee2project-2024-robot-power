import cv2 as cv
import numpy as np
from helper_functions import helper_functions as hf


cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Error when connecting to camera")
    exit(1)

lower_blue, upper_blue = hf.colour_2_hsv('blue')
lower_green, upper_green = hf.colour_2_hsv('green')

while True:
    ret, frame = cap.read()
    if not ret:
        print("Stream Down")
        exit(1)
    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
    mask_blue = cv.inRange(hsv, lower_blue, upper_blue)
    mask_green = cv.inRange(hsv, lower_green, upper_green)
    mask = cv.bitwise_or(mask_blue, mask_green)
    res = cv.bitwise_and(frame, frame, mask=mask)
    cv.imshow('Raw', frame)
    # cv.imshow("hsv", hsv)
    cv.imshow('Mask - Green', mask_green)
    cv.imshow('Mask - Blue', mask_blue)
    cv.imshow('Mask', mask)
    cv.imshow('res', res)
    k = cv.waitKey(30) & 0xff
    if k == ord('q'):
        break

cv.destroyAllWindows()