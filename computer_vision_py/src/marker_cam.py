import cv2 as cv
# import cv2.aruco as arucoAruco_Parameters_Default
import numpy
import sys

cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Could not open camera")
    exit()

detector_params = cv.aruco.DetectorParameters()
dictionary = cv.aruco.Dictionary

while True:
    ret, frame = cap.read()
    if not ret:
        print("Could not read frame. Exiting...")
        break
    (corners, ids, rejectedImgPoints) = cv.aruco.detectMarkers(frame, dictionary=dictionary, parameters=detector_params)
    print(corners)
    print(ids)
    print(rejectedImgPoints)




    if cv.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv.destroyAllWindows()