#!/usr/bin/python
# -*- coding:utf-8 -*-

import cv2
import numpy as np

img = cv2.imread('l1h2.png')
b, g, r = cv2.split(img)
img2 = b - g
img2[img2 <= 249] = 0
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
canny = cv2.Canny(gray, 50, 100)
kernel = np.ones((5, 5), np.uint8)
dilate = cv2.dilate(canny, kernel, iterations=2)

h, w = img.shape[:2] # inspired by Prof.Gu
mask = np.zeros([h+2, w+2], np.uint8) # inspired by Prof.Gu
cv2.floodFill(dilate, mask, (0, 0), (255, 255, 255))  # inspired by Prof.Gu / Floodfill algorithm

ret, binary = cv2.threshold(dilate, 127, 255, cv2.THRESH_BINARY)
image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
ret, binary = cv2.threshold(img2, 127, 255, cv2.THRESH_BINARY_INV)
imageb, contoursb, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
drawing = cv2.cvtColor(img2, cv2.COLOR_GRAY2BGR)
draw = cv2.cvtColor(dilate, cv2.COLOR_GRAY2BGR)
cv2.drawContours(draw, contours[:-1], -1, (0, 0, 255), 3)
# find the coordinates of lestmost blue point and right most blue point
x1 = int(cv2.moments(contoursb[1])["m10"] / cv2.moments(contoursb[1])["m00"])
x2 = int(cv2.moments(contoursb[2])["m10"] / cv2.moments(contoursb[2])["m00"])
y1 = int(cv2.moments(contoursb[1])["m01"] / cv2.moments(contoursb[1])["m00"])
y2 = int(cv2.moments(contoursb[2])["m01"] / cv2.moments(contoursb[2])["m00"])

# draw the contour and center of the shape on the image
cv2.drawContours(drawing, contoursb[:-1], -1, (0, 0, 255), 3)
cv2.circle(drawing, (x1, y1), 7, (0, 0, 255), -1)
cv2.circle(drawing, (x2, y2), 7, (0, 0, 255), -1)
cv2.line(drawing, (x1, y1), (x2, y2), (0, 0, 255), 3)
# print the numbers of all objectives and blue objectives
print("There are %d objectives, and %d is blue." % (len(contours)-1, len(contoursb)-1))
cv2.imshow('L1H2-Task1', draw)
cv2.imshow('L1H2-Task2', drawing)
cv2.waitKey(0)
cv2.destroyAllWindows()
