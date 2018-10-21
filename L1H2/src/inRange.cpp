#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

int main(int argc, char const *argv[])
{
    cv::Mat src;
    src = cv::imread("folder/1.png");

    if (src.empty())
    {
        std::cout << "img is empty" << std::endl;
        return -1;
    }

    //the inRange method better than using the split method. sometimes.
    cv::Mat redBinary;
    cv::inRange(src, cv::Scalar(0,0,100), cv::Scalar(40,30,255), redBinary);

    cv::Mat imgR = src.clone();
    imgR.setTo(255, redBinary);

    cv::Mat grayR;
    cv::cvtColor(imgR, grayR, cv::COLOR_BGR2GRAY);
    cv::threshold(grayR, grayR, 254, 255, CV_THRESH_BINARY);

    cv::Mat dilateImg, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));
    cv::dilate(grayR, dilateImg, kernel, cv::Point(-1, -1), 1);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(dilateImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    int couter = contours.size() - 1;
    cv::findContours(redBinary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    std::cout << "Objects : " << contours.size() + couter << std::endl;
    
    cv::Mat blueBinary;
    cv::inRange(src, cv::Scalar(100,0,0), cv::Scalar(255,30,40), blueBinary);
    cv::findContours(blueBinary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    std::cout << "blue ball : " << contours.size() << std::endl;

    std::vector<cv::Point> leftBall = contours[1];
    std::vector<cv::Point> rightBall = contours[2];

    float _;
    cv::Point2f centerBallLeft;
    cv::Point2f centerBallRight;

    cv::minEnclosingCircle(leftBall, centerBallLeft, _);
    cv::minEnclosingCircle(rightBall, centerBallRight, _);

    cv::circle(src, centerBallLeft, 1, cv::Scalar(0, 255, 0), 3);
    cv::circle(src, centerBallRight, 1, cv::Scalar(0, 255, 0), 3);
    cv::circle(src, cv::Point(
            (int)((centerBallLeft.x + centerBallRight.x) / 2), 
            (int)((centerBallLeft.y + centerBallRight.y) / 2)
        ), 3, cv::Scalar(0, 0, 255), 3);

    while (true)
    {
        cv::imshow("src", src);

        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
