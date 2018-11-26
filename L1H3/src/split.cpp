#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

bool isSquare(Rect rect, float thresh){
    return (rect.width > rect.height * (1 - thresh)) &&
           (rect.width < rect.height * (1 + thresh));
}

int main(int argc, char const *argv[])
{
    VideoCapture capture("folder/1.mp4");

    if (!capture.isOpened())
    {
        cout << "fail to open" << endl;
        return -1;
    }

    long maxFrame = capture.get(CV_CAP_PROP_FRAME_COUNT);
    cout << "maxFrame : " << maxFrame << endl;

    Mat src, kernel;
    kernel = getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));

    while (true)
    {
        capture >> src;

        if (src.empty())
        {
            cout << "video is over" << endl;
            return -1;
        }

        Mat G;
        vector<cv::Mat> bgrChannels;
        split(src, bgrChannels);
        G = bgrChannels.at(1);

        imshow("g", G);

        Mat binary;
        threshold(G, binary, 254, 255, THRESH_BINARY);

        imshow("binary", binary);

        dilate(binary, binary, kernel, Point(-1, -1), 3);

        vector<vector<Point>> contours;
        findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Mat drawing;
        cvtColor(binary, drawing, cv::COLOR_GRAY2BGR);

        // contours.size() -1  is means that abandon the biggest one(the last one)
        for (size_t idx = 0; idx < contours.size(); idx++)
        {
            Rect square;
            square = boundingRect(contours[idx]);
            if (isSquare(square, 0.5))
                rectangle(drawing, square, Scalar(0, 255, 0), 2);
        }

        imshow("binary", binary);
        imshow("drawing", drawing);

        int cur = capture.get(CV_CAP_PROP_POS_FRAMES);
        if (cur == maxFrame)
            capture.set(CV_CAP_PROP_POS_FRAMES, 0);

        if (waitKey(0) == 27)
            break;
    }

    destroyAllWindows();
    capture.release();
    return 0;
}
