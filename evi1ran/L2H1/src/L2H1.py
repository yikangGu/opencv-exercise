import numpy as np
import cv2


def main():
    # capture the video from the local directory
    cap = cv2.VideoCapture("L2H1.mp4")
    # if the video is existed    
    x_list = []
    y_list = []
    pre_x = 0
    pre_y = 0
    while(cap.isOpened()):
        # read the frame from the video
        grabbed, frame = cap.read()
        # denoise the frame by Gaussian Blur Algorithm
        blur = cv2.GaussianBlur(frame, (27, 27), 0)
        # convert the frame from BGR to HSV
        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV) 
        # separate the Green ball
        lower = np.array([35, 43, 46]) 
        upper = np.array([77, 255, 255])  
        green = cv2.inRange(hsv, lowerb=lower, upperb=upper)
        # binarization processing
        ret, binary = cv2.threshold(green, 127, 255, cv2.THRESH_BINARY)
        image, contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
        # extract the green ball
        for c in contours:
            (x, y, w, h) = cv2.boundingRect(c) 
            cv2.circle(frame, (x+w/2, y+h/2), 2, (0, 0, 255), 2)
            # store the coordinates of previous two points
            if (len(x_list) < 2 and len(y_list) < 2):
                x_list.append(x+w/2)
                y_list.append(y+h/2)
            else:
                # lists to implement queues
                x_list.pop(0)
                y_list.pop(0)
                x_list.append(x+w/2)
                y_list.append(y+h/2)
        if (len(x_list) == 2 and len(y_list) == 2):
            
            #  2-D accelerate vector prediction  - inspired by Prof.Gu
            #   X = 2*X(n) - X(n-1)
            #  Set each frame t = 1
            #  then a = (X(n) - X(n-1)) / t , (t=1)
            #  because V(t) = V(0) + a*t
            #  we know that X(n+1) = X(n) + X(n) - X(n-1)
            pre_x = 2 * x_list[1] - x_list[0]
            pre_y = 2 * y_list[1] - y_list[0]
            cv2.circle(frame, (pre_x,pre_y), 2, (255, 0, 0), 2)
        # show the video
        cv2.imshow("L2H1", frame)
        if cv2.waitKey(100) == 27:
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
