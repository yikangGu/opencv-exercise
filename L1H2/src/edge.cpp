#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

int main(int argc, char const *argv[])
{
    cv::Mat src;
    src = cv::imread("img/1.png");

    if (src.empty())
    {
        std::cout << "img is empty" << std::endl;
        return -1;
    }

    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

    cv::Mat canny;
    cv::Canny(gray, canny, 50, 100);

    cv::Mat dilate, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));
    cv::dilate(canny, dilate, kernel, cv::Point(-1, -1), 1);

    // replace the inRange method and the loop over method when i found the flood fill method.
    // then the edge.cpp created.
    cv::floodFill(dilate, cv::Point(0, 0), cv::Scalar(255));

    std::vector <std::vector<cv::Point>> contours;
    cv::findContours(dilate, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    std::cout << "Objects : " << contours.size() - 1 << std::endl;

    cv::Mat blueBinary;
    cv::inRange(src, cv::Scalar(100, 0, 0), cv::Scalar(255, 30, 40), blueBinary);
    cv::findContours(blueBinary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    std::cout << "blue ball : " << contours.size() << std::endl;

    std::vector <cv::Point> leftBall  = contours[1];
    std::vector <cv::Point> rightBall = contours[2];

    float       _;
    cv::Mat     drawing               = src.clone();
    cv::Point2f centerBallLeft;
    cv::Point2f centerBallRight;

    cv::minEnclosingCircle(leftBall, centerBallLeft, _);
    cv::minEnclosingCircle(rightBall, centerBallRight, _);

    cv::circle(src, centerBallLeft, 1, cv::Scalar(0, 255, 0), 3);
    cv::circle(src, centerBallRight, 1, cv::Scalar(0, 255, 0), 3);
    cv::circle(src, cv::Point((int) ((centerBallLeft.x + centerBallRight.x) / 2), (int) ((centerBallLeft.y + centerBallRight.y) / 2)), 3, cv::Scalar(0, 0, 255), 3);

    while (true)
    {
        cv::imshow("src", src);
        if (cv::waitKey(1) == 27) break;
    }

    cv::destroyAllWindows();
    return 0;
}
