import cv2
import numpy as np

photosdone = False
#get dicntonayl
markers = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)

#define camerea
camera = cv2.VideoCapture()
#open first camera
camera.open(0)

#create aruco board
charucoBoard = cv2.aruco.CharucoBoard_create(7, 5, 0.04, 0.03, markers)

#create video window
cv2.namedWindow("Video")
#define img
frame = 0
#get size
rec = 0 
allCorners = []
allIds = []
#allow for camera aligmnet
print("algin camera and then press enter to conferm")
while not photosdone:
    #give person chance to lien up shot
    rec, frame = camera.read()
    framedup = frame
    #detect markers
    markerCorners, markerIds, rejPoints = cv2.aruco.detectMarkers(frame, markers)
    #get corners
    if len(markerCorners) >  0:
        #cacluate corners
        charucoIds = []
        charucoCorners = []
        [retval, charucoCorners, charucoIds] = cv2.aruco.interpolateCornersCharuco(markerCorners, markerIds, frame, charucoBoard)
        #if corners > 0 draw corners
        if charucoCorners is not None and charucoIds is not None: 
            
            framedup = cv2.aruco.drawDetectedCornersCharuco(framedup, charucoCorners, charucoIds)
            #also if enter is pressed add to arrya coners and stuff
            if cv2.waitKey(3) == ord("e"):
                allCorners.append(charucoCorners)
                allIds.append(charucoIds)
                print("saving")
        #darw marker
        #framedup = cv2.aruco.drawDetectedMarkers(framedup, markerCorners, markerIds)
        
    cv2.imshow("Video", framedup)
    #excpate key to stop
    if cv2.waitKey(3) == 27: photosdone = True 

#return values
cameraMatrix = 0
distCoeffs = 0
#get calabration vlaues 
retval, cameraMatrix, distCoeffs, rvecss, tvecss = cv2.aruco.calibrateCameraCharuco(allCorners, allIds, charucoBoard, frame.shape[1::-1], None, None)
#


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
        image, markers, corners)
    #dectect pose
    rvecs, tvecs, _objPoints = cv2.aurco.estimatePoseSingleMarkers(corners, 2, cameraMatrix, distCoeffs)
    #draw detected markes
    imageCopy = cv2.aruco.drawDetectedMarkers(imageCopy, corners, ids)
    imageCopy = cv2.aruco.drawAxis(imageCopy, cameraMatrix, distCoeffs, rvecs, tvecs, 0.1)
    #show on windows
    cv2.imshow("Video", imageCopy)
    #relace
    key = cv2.waitKey(30)
    if key > 0:
        break

camera.release()
cv2.destroyAllWindows() 