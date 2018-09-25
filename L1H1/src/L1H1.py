import cv2


def main():
    img = cv2.imread("folder/fig1.jpg")

    while True:
        if cv2.waitKey(1) == 27:
            break

        cv2.imshow("img", img)

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
