import cv2
import numpy as np

#set icon wdiht in meters
markerwdith = 0.0525

#inputvideo file

camera = cv2.VideoCapture()
camera.open(0)
if not camera.isOpened():
    raise Exception("Camera not found")

#create marker compution
markers = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)
#create video
cv2.namedWindow("Video")

while True:
    #define images
    image = 0
    imageCopy = 0
    ret, image = camera.read()
    #copy to iamgecopy 
    imageCopy = image
    #detectmarkers and outputs imagase
    #setuop corents
    corners = []
    ids = []
    rejectedimgpoints = []

    #detect markers
    corners, ids, rejectedimgpoints = cv2.aruco.detectMarkers(
        image, markers, corners)
    #dectect pose
    
    #draw detected markes
    cv2.aruco.drawDetectedMarkers(imageCopy, corners, ids)
    #show on windows
    cv2.imshow("Video", imageCopy)
    #relace
    key = cv2.waitKey(30)
    if key > 0:
        break
camera.release()
cv2.destroyAllWindows() 
