import numpy as np
import cv2
import oe


def main():
    isFollowing = False

    curBallRoi = 0
    preBallRoi = 0
    preBalls = []
    curTarget = (0, 0)
    preTarget = (0, 0)

    drawing = 0
    counter = 0

    cap = cv2.VideoCapture("img/1.mp4")

    if not cap.isOpened():
        print(" fail to open cap ")
        return -1

    while True:
        cur_frame_idx = cap.get(cv2.CAP_PROP_POS_FRAMES)
        print(" cur: " + str(cur_frame_idx), end="")
        _, src = cap.read()

        if src is None:
            print("img is empty")
            print(" ***--- ACCURACY:", round(
                counter * 100.0/cur_frame_idx, 2), "% ---***")
            counter = 0
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
            continue

        hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
        G = cv2.inRange(hsv, minG, maxG)
        G = cv2.erode(G, kernel, iterations=1)
        G = cv2.dilate(G, kernel, iterations=1)

        if isFollowing is True:
            drawing = cv2.cvtColor(G, cv2.COLOR_GRAY2BGR)
            hs, he, ws, we = oe.extendRect(preBallRoi, G.shape, 0, 2)
            curBallRoi = G[hs:he, ws:we]

            contours, _ = cv2.findContours(
                curBallRoi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE, offset=(ws, hs))

            checkFind = False
            for contour in contours:
                rect = cv2.boundingRect(contour)

                if oe.approxLenth(rect[2:4], 0.5):
                    cv2.rectangle(drawing, (rect[0], rect[1]), (
                        rect[0]+rect[2], rect[1]+rect[3]), (0, 0, 255))

                    preBalls.append((rect[0]+rect[2]//2, rect[1]+rect[3]//2))
                    checkFind = True
                    isFollowing = True
                    preBallRoi = rect
                    counter += 1
                    print(" state: 2 following ")

            if checkFind is False:
                print(" state: 3 missing ")
                isFollowing = False
                preBalls.clear()
        else:
            drawing = cv2.cvtColor(G, cv2.COLOR_GRAY2BGR)

            contours, _ = cv2.findContours(
                G, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            checkFind = False
            for contour in contours:
                rect = cv2.boundingRect(contour)

                if oe.approxLenth(rect[2:4], 0.5):
                    cv2.rectangle(drawing, (rect[0], rect[1]), (
                        rect[0]+rect[2], rect[1]+rect[3]), (0, 0, 255))

                    preBalls.append((rect[0]+rect[2]//2, rect[1]+rect[3]//2))
                    checkFind = True
                    isFollowing = True
                    preBallRoi = rect
                    print(" state: 1 find ")

            if checkFind is False:
                print(" state: -1 can't find ")

        if len(preBalls) is 2:
            curTarget = oe.get2vsv(preBalls[1], preBalls[0])

            cv2.circle(drawing, (preBallRoi[0]+preBallRoi[2]//2,
                                 preBallRoi[1]+preBallRoi[3]//2), 2, (255, 0, 0), -1)
            cv2.circle(drawing, curTarget, 2, (0, 0, 255), -1)
            cv2.circle(drawing, preTarget, 2, (0, 255, 0), -1)

            preTarget = curTarget
            preBalls.pop(0)

        print(" target: x:", curTarget[0], " y:", curTarget[1])
        cv2.imshow("G", G)
        cv2.imshow("drawing", drawing)

        if curBallRoi is not None:
            cv2.imshow("curBallRoi", curBallRoi)

        if cv2.waitKey(0) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    minG = (35, 43, 46)
    maxG = (77, 255, 255)

    kernel = np.ones((7, 7), np.uint8)
    main()
