import cv2 as cv
import numpy as np
from helper_functions import helper_functions as iwr

"""
TODO: 
Code joint gaussian pdf
- rho needs to be computed
"""


def joint_normal(img1):
    sd = (250, 250)
    mean = (0, 0)

    constant = 1 / (2*np.pi*sd[0]*sd[1])

shape = (300, 300)
img = np.zeros(shape)
origin = (shape[0] // 2, shape[1] // 2)

constant = -(np.log(256) - np.log(10)) / 500

for i in range(0, shape[0]):
    for j in range(0, shape[1]):
        x = j - origin[0]
        y = i - origin[1]
        img[i][j] = np.exp(constant * (x ** 2 + y ** 2))

cv.imshow('Checker', img)
k = cv.waitKey(0)

if k == ord('s'):
    iwr.save_image(img, "sin")
