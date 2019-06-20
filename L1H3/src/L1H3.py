import numpy as np
import cv2


def maxLimiter(x, max_x):
    return max_x if x > max_x else x


def minLimiter(x, min_x):
    return min_x if x < min_x else x


def reduce(value, factor):
    return value * (1 - factor)


def enlarge(value, factor):
    return value * (1 + factor)


def isSquare(shape, factor):
    return (shape[0] > reduce(shape[1], factor)) and (shape[0] < enlarge(shape[1], factor))


def get2vsv(pre, cur):
    return (2 * pre[0] - cur[0], 2 * pre[1] - cur[1])


def extendRect(rect, shape, mode, factor=0.1):
    '''
    extendRect(rect, shape, mode, factor) -> retval.
    @param shape : h, w, (e.g. src.shape)
    @param mode : if mode = 0, extend by multiply a factor, if mode = 1, extend by add a factor
    '''
    max_h, max_w = shape[:2]
    extend_w, increment_w, extend_h, increment_h = 0, 0, 0, 0

    if mode is 0:
        extend_w, extend_h = enlarge(rect[2], factor), enlarge(rect[3], factor)
        increment_w, increment_h = extend_w - rect[2], extend_h - rect[3]

    elif mode is 1:
        extend_w, extend_h = rect[2] + factor, rect[3] + factor
        increment_w, increment_h = factor, factor

    else:
        print(" extendRect MODE ERROR! ")

    w_start = int(minLimiter(rect[0] - increment_w, 0))
    w_end = int(maxLimiter(rect[0] + extend_w, max_w))
    h_start = int(minLimiter(rect[1] - increment_h, 0))
    h_end = int(maxLimiter(rect[1] + extend_h, max_w))

    return (h_start, h_end, w_start, w_end)


def main():
    isFollowing = False

    curBallRoi = None
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
            print(" accuracy: ", round(
                counter * 100.0 / cur_frame_idx, 2), "%", end='')
            counter = 0
            cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
            continue

        hsv = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
        G = cv2.inRange(hsv, minG, maxG)
        G = cv2.erode(G, kernel, iterations=1)
        G = cv2.dilate(G, kernel, iterations=1)

        if isFollowing is True:
            drawing = cv2.cvtColor(G, cv2.COLOR_GRAY2BGR)
            hs, he, ws, we = extendRect(preBallRoi, G.shape, 0, 2)
            curBallRoi = G[hs:he, ws:we]

            contours, _ = cv2.findContours(
                curBallRoi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE, offset=(ws, hs))

            checkFind = False
            for contour in contours:
                rect = cv2.boundingRect(contour)

                if isSquare(rect[2:4], 0.5):
                    cv2.rectangle(drawing, (rect[0], rect[1]), (
                        rect[0] + rect[2], rect[1] + rect[3]), (0, 0, 255))

                    preBalls.append((rect[0] + rect[2] // 2, rect[1] + rect[3] // 2))
                    checkFind = True
                    isFollowing = True
                    preBallRoi = rect
                    counter += 1
                    print(" following now ")

            if checkFind is False:
                print(" missing ball")
                isFollowing = False
                preBalls.clear()
        else:
            drawing = cv2.cvtColor(G, cv2.COLOR_GRAY2BGR)

            contours, _ = cv2.findContours(
                G, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            checkFind = False
            for contour in contours:
                rect = cv2.boundingRect(contour)

                if isSquare(rect[2:4], 0.5):
                    cv2.rectangle(drawing, (rect[0], rect[1]), (
                        rect[0] + rect[2], rect[1] + rect[3]), (0, 0, 255))

                    preBalls.append((rect[0] + rect[2] // 2, rect[1] + rect[3] // 2))
                    checkFind = True
                    isFollowing = True
                    preBallRoi = rect
                    print(" find ")

            if checkFind is False: print(" can't find ball")

        if len(preBalls) is 2:
            curTarget = get2vsv(preBalls[1], preBalls[0])

            cv2.circle(drawing, (preBallRoi[0] + preBallRoi[2] // 2,
                                 preBallRoi[1] + preBallRoi[3] // 2), 2, (255, 0, 0), -1)
            cv2.circle(drawing, curTarget, 2, (0, 0, 255), -1)
            cv2.circle(drawing, preTarget, 2, (0, 255, 0), -1)

            preTarget = curTarget
            preBalls.pop(0)

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
