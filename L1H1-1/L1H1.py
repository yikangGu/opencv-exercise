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
# Change the picture to gray picture
gray =  cv2.cvtColor(img2,cv2.COLOR_BGR2GRAY)
# Change the picture to binary picture by threshold function
ret, binary = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY)
# Recognize the contours
image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
# Print the number of  objectives / len of contours
print ("There are %d objectives." % len(contours))
# Show the binary picture
cv2.imshow('L1H1', binary)
cv2.waitKey(0)
cv2.destroyAllWindows()
