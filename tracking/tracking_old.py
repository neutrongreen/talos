import cv2
import csv
import numpy as np
import math
import time 

def zerovalue(zero, value):
    if zero:
        for i in range(len(zero)):
            value[i] -= zero[i]
    return value

#convert rotationmatrix to eualr angles
def yawpitchrolldecomposition(Rx):
    R = Rx[0]
    sin_x    = math.sqrt(R[2,0] * R[2,0] +  R[2,1] * R[2,1])    
    validity  = sin_x < 1e-6
    if not validity:
        z1    = math.atan2(R[2,0], R[2,1])     # around z1-axis
        x      = math.atan2(sin_x,  R[2,2])     # around x-axis
        z2    = math.atan2(R[0,2], -R[1,2])    # around z2-axis
    else: # gimbal lock
        z1    = 0                                         # around z1-axis
        x      = math.atan2(sin_x,  R[2,2])     # around x-axis
        z2    = 0                                         # around z2-axis
    return [math.degrees(z1), math.degrees(x), math.degrees(z2)]

#set icon wdiht in cm
markerWidth = 20

#inputvideo file

markers = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)

#define camerea
camera = cv2.VideoCapture(1, cv2.CAP_DSHOW)
#open first camera
camera.set(cv2.CAP_PROP_FRAME_WIDTH,1280)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
#create aruco board

#create video window
cv2.namedWindow("Video", cv2.WINDOW_NORMAL)

cameraMatrix = np.loadtxt("calb/cameraMatrix.csv", delimiter=",")
distCoeffs = np.loadtxt("calb/distCoeffs.csv", delimiter=",")
zerovalues = [0] * 50

with open("data/{}_data.csv".format(time.time()), "w+", newline='') as csvfile:
    csvfile_writer = csv.writer(csvfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
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
            image, markers, corners, None, None, None, cameraMatrix, distCoeffs)
        #chekc if any markers are present
        if len(corners) > 0:
            #estimate pose of markers
            rvecs, tvecs, _objPoints = cv2.aruco.estimatePoseSingleMarkers(corners, markerWidth, cameraMatrix, distCoeffs)
            #draw detected markes if any present
            imageCopy = cv2.aruco.drawDetectedMarkers(imageCopy, corners, ids)
            #iterate over markers (list of rvecs will be the same length as other markers)
            for i in range(len(rvecs)):
                rvec = np.matrix(rvecs[i]).reshape((3, 1))
                tvec = np.matrix(tvecs[i]).reshape((3, 1))
                R, _ = cv2.Rodrigues(rvec)
                R = np.matrix(R).T
                tvec = np.dot(-R, np.matrix(tvec))
                tvec = np.asarray(tvec)
                #inverse the perspective
                #get attute of vectors
                angle = yawpitchrolldecomposition(cv2.Rodrigues(rvecs[i]))
                #draw marker
                #  
                imageCopy = cv2.aruco.drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], markerWidth/2)
                #print amers postions
                valuesraw = [angle[0], angle[1], angle[2], tvec[0, 0], tvec[1, 0], tvec[2, 0]]
                values = zerovalue(zerovalues[ids[i][0]], valuesraw)
                
                print("marker:{:04.1f} Rot: {:04.1f}    {:04.1f}   {:04.1f} Trans: {:04.1f}     {:04.1f}    {:04.1f}                        ".format(
                    ids[i][0], values[0], values[1], values[2], values[3], values[4], values[5], end="\r"))
                        
                csvfile_writer.writerow([time.time(), ids[i][0], values[0], values[1], values[2], values[3], values[4], values[5]])
                key = cv2.waitKey(30)
                if key == 122:
                    zerovalues[ids[i][0]] = valuesraw
            #show on windows
        cv2.imshow("Video", imageCopy)
        #relace
        key = cv2.waitKey(30)
        if key == 27 :
            break
            
    camera.release()
    cv2.destroyAllWindows() 

#daft punk intenisfys 
print("\nend of line")