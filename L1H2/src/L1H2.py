import numpy as np
import cv2


def main():
    src = cv2.imread("folder/1.bmp")
    b, g, r = cv2.split(src)
    subRG = b - g

    kernel = np.ones((3, 3), np.uint8)
    erodeImg = cv2.erode(subRG, kernel, iterations=1)

    _, binary = cv2.threshold(erodeImg, 127, 255, cv2.THRESH_BINARY)
    _, contours, _ = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    print("There are %d objectives." % len(contours))

    src.

    while True:
        if cv2.waitKey(1) == 27:
            break
        cv2.imshow("subRG", binary)

    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
