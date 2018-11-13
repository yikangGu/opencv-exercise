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

    // black ball process
    cv::Mat blackBinary;
    cv::inRange(src, cv::Scalar(82, 72, 65), cv::Scalar(83, 73, 66), blackBinary);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(blackBinary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    float _;
    cv::Point2f fristBall, secondBall;
    int order[6] = {1, 0, 3, 2, 5, 4};

    for (size_t idx = 0; idx < contours.size() - 1; idx++)
    {
        cv::minEnclosingCircle(contours[order[idx]], fristBall, _);
        cv::minEnclosingCircle(contours[order[idx + 1]], secondBall, _);
        cv::line(src, cv::Point((int)fristBall.x, (int)fristBall.y),
                 cv::Point((int)secondBall.x, (int)secondBall.y), cv::Scalar(0, 0, 255));
    }

    // find map process
    cv::Mat mapsrc = src(cv::Range(0, 410),cv::Range(700, src.cols));

    cv::Mat gray;
    cv::cvtColor(mapsrc, gray, CV_BGR2GRAY);

    cv::Mat canny;
    cv::Canny(gray, canny, 0, 2);

    cv::Mat dilate, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(2, 2));
    cv::dilate(canny, dilate, kernel, cv::Point(-1, -1), 1);

    cv::Mat zeros = cv::Mat::zeros(src.rows+2, src.cols+2, CV_8UC1);
    dilate.copyTo(zeros(cv::Rect(cv::Point(699, 0), mapsrc.size())));

    cv::floodFill(src, zeros, cv::Point(750, 50), cv::Scalar(255, 255, 255));

    while (true)
    {
        cv::imshow("src", src);

        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
