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
    canny = cv2.Canny(map_roi, 0, 2)
    canny = cv2.dilate(canny, kernel)

    mask = np.zeros((src.shape[0]+2, src.shape[1]+2), np.uint8)
    mask[y:y+h, x:x+w] = canny
    cv2.floodFill(src, mask, (x, y), (255, 255, 255))

    cv2.imshow("src", src)
    cv2.imshow("canny", canny)
    cv2.imshow("ipad_roi", ipad_roi)

    while True:

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
