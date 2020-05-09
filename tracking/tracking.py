import cv2
import csv
import numpy as np
import math

#soloty to help convert rotation matrix to eualr angle , soultion was found online idk
def get_eular(Rx):
    R = Rx
    sin_x    = math.sqrt(R[2][0] * R[2][0] +  R[2][1] * R[2][1])    
    validity  = sin_x < 1e-6
    if not validity:
        z1    = math.atan2(R[2][0], R[2][1])     # around z1-axis
        x      = math.atan2(sin_x,  R[2][2])     # around x-axis
        z2    = math.atan2(R[0][2], -R[1][2])    # around z2-axis
    else: # gimbal lock
        z1    = 0                                         # around z1-axis
        x      = math.atan2(sin_x,  R[2][2])     # around x-axis
        z2    = 0                                         # around z2-axis
    return [math.degrees(z1), math.degrees(x), math.degrees(z2)]

def detect_marker(img, mId, markersDict, camMatrix, distCoef):
    #get corners 
    corners, ids, rejectedimgpoints = cv2.aruco.detectMarkers(img, markersDict, cameraMatrix = camMatrix, distCoeff = distCoef)
    #if there is a marker of id 0 return that conrer set else return non
    ids = np.squeeze(np.asarray(ids)).tolist()
    if ids == list() and mId in ids:
        return corners[ids.index(mId)]
    elif ids == 0 and not ids == None:
        return corners[mId]
    return np.asarray([])

def get_marker_pose(corners, markerWidth, cameraMatrix, distCoeffs):
    #estimate the pose of cther single corner 
    rvecs, tvecs, _objPoints = cv2.aruco.estimatePoseSingleMarkers([corners], markerWidth, cameraMatrix, distCoeffs)
    #save rogianl values for image procceesing
    iRvec = rvecs[0]
    iTvec = tvecs[0]
    #get the only entry of rvec and tvec and convert to numpy matrix that is reshaped to be 3, 1 
    rvec = np.matrix(rvecs[0]).reshape((3, 1))
    tvec = np.matrix(tvecs[0]).reshape((3, 1))
    
    #convert from marker rletaive to camer, to camera relative to marker both rvec and tvec
    #get rogues of rvec
    R, _ = cv2.Rodrigues(rvec)
    #transpose r and covert to numpie matrix
    Rt = np.matrix(R).T
    #cacluate tvec
    tvec = np.dot(-Rt, tvec)
    #convert tvec to array
    tvec = np.squeeze(np.asarray(tvec)).tolist()
    #convert rvec to eular angles for ease of use
    rvec = get_eular(np.asarray(R))
    #return both rvecx and tvec adn the inital rvec and tvec
    #rotate tvec x by rvec
    tvec[0] = math.sqrt((math.cos(math.radians(rvec[0])) * tvec[1]) - (math.sin(math.radians(rvec[0])) * tvec[0]))
    tvec[0] = math.sqrt((math.cos(math.radians(rvec[0])) * tvec[1]) + (math.sin(math.radians(rvec[0])) * tvec[0]))
    #rotate tvec y by revec
    return (rvec, tvec, iRvec, iTvec)


def track_marker(img, markerDic, markerId, markerWidth, cameraMatrix, distCoeffs):
    #detect markers
    outImage = img
    markerCorner = detect_marker(img, markerId, markerDic, cameraMatrix, distCoeffs)
    #if markers detected get pose and draw marker items 
    if markerCorner.any():
        rvec, tvec, iRvec, iTvec = get_marker_pose(markerCorner, markerWidth, cameraMatrix, distCoeffs)
        #draw markers and aixis
        outImage = cv2.aruco.drawDetectedMarkers(outImage, [markerCorner])
        outImage = cv2.aruco.drawAxis(outImage, cameraMatrix, distCoeffs, iRvec, iTvec, markerWidth/2)
        #return outimage and rvec and tvec
        return (outImage, rvec, tvec)
    #else jsut return outmiane and two nones
    return (outImage, None, None)

