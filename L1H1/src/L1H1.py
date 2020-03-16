import numpy as np
import cv2


def HH():
    img = cv2.imread("img/1.jpg", 0)

    # this is numpy array [] operator
    img[img < 2] = 255
    img[img < 255] = 0

    distance = cv2.distanceTransform(
        img, cv2.DIST_L2, 5, dstType=cv2.CV_32F)
    cv2.normalize(distance, distance, 0, 255, cv2.NORM_MINMAX)
    distance = np.uint8(distance)
    _, distance = cv2.threshold(distance, 127, 255, cv2.THRESH_BINARY)

    # find contours :
    #   RETR_LIST : to find all contours without establishing any hierarchical relationships.
    #   see : https://docs.opencv.org/3.4.1/d3/dc0/group__imgproc__shape.html#ga819779b9857cc2f8601e6526a3a5bc71
    #   CHAIN_APPROX_SIMPLE : to get less points from contours.
    #   see : https://docs.opencv.org/3.4.1/d3/dc0/group__imgproc__shape.html#ga4303f45752694956374734a03c54d5ff
    contours, hierarchy = cv2.findContours(
        distance, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    # convect grey to bgr for drawing color in the image.
    drawing = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)

    #
    cv2.drawContours(drawing, contours, -1, (0, 0, 255), -1)

    print(len(contours))

    while True:
        cv2.imshow("drawing", drawing)

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    HH()
