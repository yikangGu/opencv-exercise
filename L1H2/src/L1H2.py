import numpy as np
import cv2


def main():
    src = cv2.imread("folder/1.png")
    gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
    canny = cv2.Canny(gray, 50, 100)

    kernel = np.ones((5, 5), np.uint8)
    dilate = cv2.dilate(canny, kernel, iterations=1)

    h, w = src.shape[:2]
    mask = np.zeros([h+2, w+2], np.uint8)
    cv2.floodFill(dilate, mask, (0, 0), (255, 255, 255))

    dilate, contours, _ = cv2.findContours(
        dilate, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE
    )
    print("objects : " + str(len(contours) - 1))

    blueBinary = cv2.inRange(src, (100, 0, 0), (255, 30, 40))
    blueBinary, contours, _ = cv2.findContours(
        blueBinary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
    )
    print("blue ball : " + str(len(contours)))

    centerBallLeft, _ = cv2.minEnclosingCircle(contours[1])
    centerBallRight, _ = cv2.minEnclosingCircle(contours[2])

    cv2.circle(src, (
        int(centerBallLeft[0]),
        int(centerBallLeft[1])
    ), 1, (0, 255, 0), 3)

    cv2.circle(src, (
        int(centerBallRight[0]),
        int(centerBallRight[1])
    ), 1, (0, 255, 0), 3)

    cv2.circle(src, (
        int((centerBallLeft[0] + centerBallRight[0]) / 2),
        int((centerBallLeft[1] + centerBallRight[1]) / 2)
    ), 3, (0, 0, 255), 3)

    while True:
        cv2.imshow("src", src)

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
