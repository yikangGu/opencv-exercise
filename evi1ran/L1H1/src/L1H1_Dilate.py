#!/usr/bin/python
# -*- coding:utf-8 -*-

import cv2
import numpy as np


img = cv2.imread('L1H1.png', 0)
# numpy traverse all the elements and convert 2 to 255
img[img == 2] = 255

kernel = np.ones((5, 5), np.uint8)
dilateImg = cv2.dilate(img, kernel, iterations=6)

dilateImg, contours, hierarchy = cv2.findContours(
    dilateImg, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

drawing = cv2.cvtColor(dilateImg, cv2.COLOR_GRAY2BGR)
# Draw the contours except the biggest one (edge)
cv2.drawContours(drawing, contours[:-1], -1, (0, 0, 255), 3)

# Print the number of  objectives / len of contours
print ("There are %d objectives." % (len(contours)-1))

cv2.imshow('L1H1', drawing)
cv2.waitKey(0)
cv2.destroyAllWindows()
