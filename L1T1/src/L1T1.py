import numpy as np
import cv2


def main():
    src = cv2.imread("folder/1.png")

    while True:
        cv2.imshow("src", src)

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
