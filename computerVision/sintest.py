import cv2 as cv
import numpy as np
from helper_functions import image_wr_helper as iwr


class artist:
    def __init__(self, image_array=None):
        self.image = cv.CreateMat(image_array)
        self.shape = self.image.shape
        self.origin = (shape[0] // 2, shape[1] // 2)

    def draw_sinc(self):
        if self.image is None:
            print("Please supply image")
            exit()
        for i in range(0, self.shape[0]):
            for j in range(0, self.shape[1]):
                x = i - self.origin[0]
                y = j - self.origin[1]
                if x == 0 and y == 0:
                    img[i][j] = 256
                    continue
                img[i][j] = np.sinc(x / 10) * np.sinc(y / 10) * 256



shape = (300, 300)
img = np.zeros(shape)
origin = (shape[0] // 2, shape[1] // 2)

for i in range(0, shape[0]):
    for j in range(0, shape[1]):
        x = i - origin[0]
        y = j - origin[1]
        if x == 0 and y == 0:
            img[i][j] = 256
            continue
        img[i][j] = np.sinc(x / 10) * np.sinc(y / 10) * 256

cv.imshow('Checker', img)
k = cv.waitKey(0)

if k == ord('s'):
    iwr.save_image(img, "sinc")
