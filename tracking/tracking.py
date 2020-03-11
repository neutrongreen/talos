import cv2
import numpy as np

#inputvideo file

camera = cv2.VideoCapture()
camera.open(0)
if not camera.isOpened():
    raise Exception("Camera not found")

#create marker compution
markers = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)
#create video
cv2.namedWindow("Video")

while(camera.grab())
    #define images
    image, imagecopy = 0
    camera.retrive(image)
    #copy to iamgecopy 
    image.copyTo(imageCopy)
    #detectmarkers and outputs imagase
    #setuop corents
    cv2.aruco.detectMarkers(image, markers)
    
    corners, ids, rejectedimgpoints = cv2.aruco.detectMarkers(image, markers)
    if ids.size() > 0:
        cv2.aruco.drawDetectedMarkers(imageCopy, corners, ids)
    cv2.imshow("out", imagecopy)
    key = cv2.waitKey(30)
    if key != 0:
        break
