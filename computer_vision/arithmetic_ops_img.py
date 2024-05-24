import subprocess

import cv2 as cv
import numpy as np
from helper_functions import helper_functions as hf
import matplotlib.pyplot as plt
import os


def transition(img1, img2, index):
    for i in np.linspace(0, 1, 11, endpoint=True):
        dst = cv.addWeighted(img1, i, img2, 1 - i, 0)
        cv.imwrite(f"images/slideshow/{index}-{round(i, 2)}.jpg", dst)


cleo_list = os.listdir("images/cat")
for index in range(len(cleo_list)-1):
    img1 = cv.imread(f"images/cat/{cleo_list[index]}")
    img2 = cv.imread(f"images/cat/{cleo_list[index+1]}")
    transition(img1, img2, index)

