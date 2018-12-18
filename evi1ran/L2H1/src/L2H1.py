import numpy as np
import cv2


def main():
    # capture the video from the local directory
    cap = cv2.VideoCapture("L2H1.mp4")
    # set the first frame is None
    firstFrame = None
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
   
        # set the first frame
        if firstFrame is None:
            firstFrame = green
            continue
        # diff the current frame and the first frame
        frameDelta = cv2.absdiff(firstFrame, green)
        # binarization processing
        ret, binary = cv2.threshold(frameDelta, 127, 255, cv2.THRESH_BINARY)
        image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        # draw the point that green ball might reach
        for c in contours:
            ((x, y), radius) = cv2.minEnclosingCircle(c)   
            M = cv2.moments(c)  
            center = (int(M["m10"]/M["m00"]), int(M["m01"]/M["m00"]))  
            cv2.circle(frame, center, 1, (0, 0, 255), 3)

        # show the video
        cv2.imshow("L1H3", frame)
        if cv2.waitKey(100) == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
    

if __name__ == '__main__':
    main()
