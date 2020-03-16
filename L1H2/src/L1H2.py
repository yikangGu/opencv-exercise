import numpy as np
import cv2


def main():
    src = cv2.imread("img/1.png")

    # proc find object
    gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
    canny = cv2.Canny(gray, 50, 100)

    kernel = np.ones((3, 3), np.uint8)
    dilate = cv2.dilate(canny, kernel, iterations=1)

    h, w = src.shape[:2]
    mask = np.zeros([h+2, w+2], np.uint8)
    cv2.floodFill(dilate, mask, (0, 0), (255, 255, 255))

    dilate = 255 - dilate
    contours, _ = cv2.findContours(
        dilate, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    print("objects : " + str(len(contours)))

    # proc blue ball
    blueBinary = cv2.inRange(src, (127, 0, 0), (255, 77, 77))
    contours, _ = cv2.findContours(
        blueBinary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    print("blue ball : " + str(len(contours)))

    a = [cv2.minEnclosingCircle(contours[idx])[0]
         for idx in range(len(contours))]
    centers = np.uint16(a)

    # sorted_centers, centers_len = sorted(centers, key=lambda x: x[0]), len(centers)
    left = min(centers, key=lambda center: center[0])
    right = max(centers, key=lambda center: center[0])

    cv2.circle(src, tuple(left), 3, (0, 255, 0), 3)
    cv2.circle(src, tuple(right), 3, (0, 255, 0), 3)
    cv2.circle(src, (int((left[0] + right[0]) / 2),
                     int((left[1] + right[1]) / 2)), 3, (0, 0, 255), 3)

    ycrcb = cv2.cvtColor(src, cv2.COLOR_BGR2YCrCb)

    cv2.imshow("src", src)
    cv2.imshow("ycrcb", ycrcb)

    while True:

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
