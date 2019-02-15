#!/usr/bin/python
# -*- coding:utf-8 -*-

import cv2
import numpy as np

# read the picture
img = cv2.imread('L1T1.png')
# display the map
img[img==225] = 0
# using algorithm to eliminate noise
dst = cv2.fastNlMeansDenoisingColored(img,None,2,2,7,21)
# copy to another array & strengthen the color of the balls
img2 = dst.copy()
img2[img2!=72] = 255
b, g, r = cv2.split(img2)
# create two arrays for storing the moment
x = np.zeros((7),np.int)
y = np.zeros((7),np.int)
# edge detection via canny picture
kernel = np.ones((2, 2), np.uint8)
close = cv2.morphologyEx(g, cv2.MORPH_CLOSE, kernel) 
canny = cv2.Canny(close, 50, 100)

# floodflii algorithm
h, w = img.shape[:2] 
mask = np.zeros([h+2, w+2], np.uint8) 
cv2.floodFill(canny, mask, (0, 0), (255, 255, 255)) 

# binarization processing
ret, binary = cv2.threshold(canny, 225, 255, cv2.THRESH_BINARY_INV)
image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

# storing the central point of each ball
for i in range(1,7):
    x[i] = int(cv2.moments(contours[i])["m10"] / cv2.moments(contours[i])["m00"])
for i in range(1,7):
    y[i] = int(cv2.moments(contours[i])["m01"] / cv2.moments(contours[i])["m00"])

cv2.drawContours(dst, contours, 1-6, (0, 0, 255), 1)

for i in range(1,7):
    cv2.circle(dst, (x[i], y[i]), 3, (0, 0, 255), -1)

for i in range(1,6):
    cv2.line(dst, (x[i], y[i]), (x[i+1], y[i+1]), (0, 0, 255), 1)

cv2.imshow('L1T1', dst)
cv2.waitKey(0)
cv2.destroyAllWindows()
