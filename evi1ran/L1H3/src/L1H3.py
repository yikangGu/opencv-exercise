import numpy as np
import cv2


def main():
    # capture the video from the local directory
    cap = cv2.VideoCapture("L1H3.mp4")
    # if the video is existed    
    while(cap.isOpened()):
        # read the frame from the video
        grabbed, frame = cap.read()
        # denoise the frame by Gaussian Blur Algorithm
        blur = cv2.GaussianBlur(frame, (21, 21), 0)
        # convert the frame from BGR to HSV
        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV) 
        # separate the Green ball
        lower = np.array([35, 43, 46]) 
        upper = np.array([77, 255, 255])  
        green = cv2.inRange(hsv, lowerb=lower, upperb=upper)
        # binarization processing
        ret, binary = cv2.threshold(green, 127, 255, cv2.THRESH_BINARY)
        image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        # draw the rectangle around the green ball
        for c in contours:
            (x, y, w, h) = cv2.boundingRect(c) 
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 2)
        # show the video
        cv2.imshow("L1H3", frame)
        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()
    cap.release()

if __name__ == '__main__':
    main()
