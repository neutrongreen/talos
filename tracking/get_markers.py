#needs opencv2 python and contrib modules
import cv2
import numpy as np
#create markets

#arco marker dictonary
markers = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_7X7_50)
#set image to null and draw marker
for i in range(3):
    img = 0
    img = cv2.aruco.drawMarker(markers, i, 200, img, 1)
    cv2.imshow("image: {a}".format(a=i), img)
    cv2.imwrite("markers/marker{a}.png".format(a=i), img)

#wait unir pass
while cv2.waitKey(30) <= 0: pass
cv2.destroyAllWindows()
