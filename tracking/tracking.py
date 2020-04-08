import cv2
import numpy as np
import math

#inverse the pespective

# Experimental inversion
def inversePerspectiveWithTransformMatrix(tvec, rvec):
    R, _ = cv2.Rodrigues(rvec)  # 3x3 representation of rvec
    R = np.matrix(R).T  # transpose of 3x3 rotation matrix
    transformMatrix = np.zeros((4, 4), dtype=float)  # Transformation matrix
    # Transformation matrix fill operation, matrix should be [R|t,0|1]
    transformMatrix[0:3, 0:3] = R
    transformMatrix[0:3, 3] = tvec
    transformMatrix[3, 3] = 1
    # Inverse the transform matrix to get camera centered Transform
    _transformMatrix = np.linalg.inv(transformMatrix)
    # Extract new rotation and translation vectors from transform matrix
    _R = _transformMatrix[0:3, 0:3]
    _tvec = _transformMatrix[0:3, 3]
    _rvec, _ = cv2.Rodrigues(_R)
    # return
    return _tvec, _rvec

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
markerWidth = 5.2

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

#TESTING LOOP, WILL BE REMOVED
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
            #inverse the perspective
            intvec, inrvec = inversePerspectiveWithTransformMatrix(tvecs[i], rvecs[i])
            #get attute of vectors
            angle = yawpitchrolldecomposition(cv2.Rodrigues(intvec))
            #draw marker 
            imageCopy = cv2.aruco.drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], markerWidth/2)
            #print amers postions
            print("marker{i:0.2f}: {x:0.2f}:{y:0.2f}".format(i=i,x=angle, y=intvec), end="\r")
        #show on windows
    cv2.imshow("Video", imageCopy)
    #relace
    key = cv2.waitKey(30)
    if key > 0:
        break
camera.release()
cv2.destroyAllWindows() 

#daft punk intenisfys 
print("end of line")