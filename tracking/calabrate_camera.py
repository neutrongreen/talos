import cv2
import numpy as np

photosdone = False
#get dicntonayl
markers = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)

#define camerea
camera = cv2.VideoCapture(1, cv2.CAP_DSHOW)
#open first camera
camera.set(cv2.CAP_PROP_FRAME_WIDTH,1280)
camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
#create aruco board
charucoBoard = cv2.aruco.CharucoBoard_create(7, 5, 0.04, 0.03, markers)

#create video window
cv2.namedWindow("Video", cv2.WINDOW_NORMAL)

#define img
frame = 0
#get size
rec = 0 
allCorners = []
allIds = []
#allow for camera aligmnet
print("algin camera and then press e to save, and esc to exit and clerbrate")
while not photosdone:
    #give person chance to lien up shot
    camera.grab()
    rec, frame = camera.retrieve()
    framedup = frame
    #detect markers
    if frame is not None:

        markerCorners, markerIds, rejPoints = cv2.aruco.detectMarkers(frame, markers)
        #get corners
        if len(markerCorners) >  0:
            #cacluate corners
            charucoIds = []
            charucoCorners = []
            [retval, charucoCorners, charucoIds] = cv2.aruco.interpolateCornersCharuco(markerCorners, markerIds, frame, charucoBoard)
            #if corners exist tjhen draw
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
print("calbrating")
retval, cameraMatrix, distCoeffs, rvecss, tvecss = cv2.aruco.calibrateCameraCharuco(allCorners, allIds, charucoBoard, frame.shape[1::-1], None, None)
#save data
np.savetxt("calb/cameraMatrix.csv", cameraMatrix, delimiter=",")
np.savetxt("calb/distCoeffs.csv", distCoeffs, delimiter=",")

#exit clabration
camera.release()
cv2.destroyAllWindows() 