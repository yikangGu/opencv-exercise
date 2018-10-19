 import numpy as np
import cv2


def main():
    img = cv2.imread("folder/1.jpg", 0)

    # this is numpy array [] operator
    img[img == 2] = 255

    img = img.copy()
    kernel = np.ones((5, 5), np.uint8)
    dilateImg = cv2.dilate(img, kernel, iterations=10)

    # find contours : 
    #   RETR_LIST : to find all contours without establishing any hierarchical relationships.
    #   see : https://docs.opencv.org/3.4.1/d3/dc0/group__imgproc__shape.html#ga819779b9857cc2f8601e6526a3a5bc71
    #   CHAIN_APPROX_SIMPLE : to get less points from contours.
    #   see : https://docs.opencv.org/3.4.1/d3/dc0/group__imgproc__shape.html#ga4303f45752694956374734a03c54d5ff
    dilateImg, contours, hierarchy = cv2.findContours(
        dilateImg, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    # convect grey to bgr for drawing color in the image.
    drawing = cv2.cvtColor(dilateImg, cv2.COLOR_GRAY2BGR)

    # slice([:-1]) is means that abandon the biggest one(the last one)
    cv2.drawContours(drawing, contours[:-1], -1, (0, 255, 0), 1)
    print(len(contours) - 1)

    while True:
        if cv2.waitKey(1) == 27:
            break
        cv2.imshow("img", drawing)

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
