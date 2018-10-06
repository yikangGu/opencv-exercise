#!/usr/bin/python
# -*- coding:utf-8 -*-

import cv2
import numpy as np

# Read the picture
img = cv2.imread('Q1.png')
# Separate the RGB plane
b, g, r = cv2.split(img)
# Merge a new picture to show the objectives
img2 = cv2.merge([b-1,g-1,r-1])
kernel = np.ones((5, 5), np.uint8)
erodeImg = cv2.erode(img2, kernel, iterations=6)
# Change the picture to gray picture
gray =  cv2.cvtColor(erodeImg,cv2.COLOR_BGR2GRAY)
# Change the picture to binary picture by threshold function   ** BINARY_INV Convert black to white, Get the true contours
ret, binary = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY_INV)
# Recognize the contours
image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
# Print the number of  objectives / len of contours
# ** Delete the edge(the biggest one) **

# Draw the contours except the biggest one (edge)
cv2.drawContours(erodeImg, contours[:-1], -1, (0, 0, 255), 3)
print ("There are %d objectives." % (len(contours)-1))
# Show the binary picture
cv2.imshow('L1H1', erodeImg)
cv2.waitKey(0)
cv2.destroyAllWindows()
