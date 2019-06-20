import numpy as np
import cv2
import oe


def main():
    src = cv2.imread("img/1.png")

    if not src.any():
        print("img is empty")

    kernel = np.ones((3, 3), np.uint8)
    gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)

# black ball proc
    canny = cv2.Canny(gray, 127, 255)
    hole = oe.Hole(canny)
    hole = cv2.erode(hole, kernel, iterations=2)

    # find ipad proc
    contours, _ = cv2.findContours(
        hole, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    candidate_rect = [rotateRect for rotateRect in
                      [cv2.minAreaRect(contour) for contour in contours]
                      if 50 < abs(rotateRect[2]) < 70]

    if not len(candidate_rect):
        print("no match")

    # find ball proc
    x, y, w, h = oe.boundingRect(candidate_rect[0])
    ipad_roi = canny[y:y+h, x:x+w]
    ipad_roi = oe.Hole(ipad_roi, 1)

    contours, _ = cv2.findContours(
        ipad_roi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE, offset=(x, y))

    candidate_ball = [rotateRect for rotateRect in
                      [cv2.minAreaRect(contour) for contour in contours]
                      if rotateRect[2] == 0]

    if not len(candidate_ball):
        print("no match")

    # sort and draw
    sorted_balls = sorted(candidate_ball, key=lambda ball: ball[0][0])
    for ball in sorted_balls:
        x, y, w, h = oe.boundingRect(ball)
        cv2.rectangle(src, (x, y), (x+w, y+h), (0, 0, 255))

    for idx in range(len(sorted_balls)-1):
        cv2.line(src,
                 (int(sorted_balls[idx][0][0]), int(sorted_balls[idx][0][1])),
                 (int(sorted_balls[idx+1][0][0]),
                  int(sorted_balls[idx+1][0][1])),
                 (0, 0, 255))

# map proc
    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)
    binary = cv2.erode(binary, kernel)

    # find map proc
    contours, _ = cv2.findContours(
        binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    maxArea, maxArea_idx = 0, 0
    for idx in range(len(contours)):
        area = cv2.contourArea(contours[idx])
        if maxArea < area:
            maxArea = area
            maxArea_idx = idx

    x, y, w, h = cv2.boundingRect(contours[maxArea_idx])
    map_roi = gray[y:y+h, x:x+w]

    # show and draw
    map_roi = cv2.Canny(map_roi, 0, 2)
    map_roi = cv2.dilate(map_roi, kernel)

    mask = np.zeros((src.shape[0]+2, src.shape[1]+2), np.uint8)
    mask[y:y+h, x:x+w] = map_roi
    cv2.floodFill(src, mask, (x, y), (255, 255, 255))

# swap computer monitor
    canny = cv2.Canny(gray, 127, 255)
    canny = cv2.dilate(canny, kernel)

    contours, hierarchy = cv2.findContours(
        canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
    for i in range(len(contours)):
        for j in range(i + 1, len(contours)):
            if hierarchy[0][i][2] == -1 or hierarchy[0][i][3] == -1:
                break

            contour_i = cv2.boundingRect(contours[i])
            contour_j = cv2.boundingRect(contours[j])

            if contour_i[2] < 100 or contour_i[3] < 100:
                break

            if contour_j[2] < 100 or contour_j[3] < 100:
                continue

            if cv2.pointPolygonTest(contours[i], (contour_j[0]+(contour_j[2]//2), contour_j[1]+(contour_j[3]//2)), False) != -1:
                continue

            if not oe.approxLenth((contour_i[2], contour_j[2]), 0.1):
                continue

            if not oe.approxLenth((contour_i[3], contour_j[3]), 0.1):
                continue

            source = cv2.matchShapes(
                contours[i], contours[j], cv2.CONTOURS_MATCH_I1, 0)

            if source == 0 or source > 1:
                continue

            left, right = (contour_i, contour_j) if contour_i[0] < contour_j[0] else (contour_j, contour_i)

    left_monitor = src[left[1]:left[1]+left[3], left[0]:left[0]+left[2]]
    right_monitor = src[right[1]:right[1] +
                        right[3], right[0]:right[0]+right[2]].copy()

    src[right[1]:right[1]+right[3], right[0]:right[0]+right[2]] = left_monitor
    src[left[1]:left[1]+left[3], left[0]:left[0]+left[2]] = right_monitor

    cv2.rectangle(
        src, (left[0], left[1]), (left[0]+left[2], left[1]+left[3]), (0, 255, 0))
    cv2.rectangle(
        src, (right[0], right[1]), (right[0]+right[2], right[1]+right[3]), (255, 0, 0))

    cv2.imshow("src", src)
    cv2.imshow("map_roi", map_roi)
    cv2.imshow("ipad_roi", ipad_roi)

    while True:
        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
