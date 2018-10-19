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

    cv::Mat B, G, R;
    std::vector<cv::Mat> bgrChannels;
    cv::split(src, bgrChannels);
    R = bgrChannels.at(2);
    G = bgrChannels.at(1);
    B = bgrChannels.at(0);

    cv::Mat subRB, binary;
    subRB = R - B;
    cv::threshold(subRB, binary, 100, 255, cv::THRESH_BINARY);
    cv::Mat imgBG = src.clone();
    imgBG.setTo(255, binary);

    cv::Mat grayImg;
    cv::cvtColor(imgBG, grayImg, cv::COLOR_BGR2GRAY);

    cv::Mat dilateImg, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));
    cv::dilate(grayImg, dilateImg, kernel, cv::Point(-1, -1), 1);

    // find edge
    cv::Mat canny;
    cv::Canny(dilateImg, canny, 100, 200);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(canny, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    // convect grey to bgr for drawing color in the image.
    cv::Mat drawing;
    cv::cvtColor(canny, drawing, cv::COLOR_GRAY2BGR);

    // contours.size() -1  is means that abandon the biggest one(the last one)
    for (size_t idx = 0; idx < contours.size() - 1; idx++)
    {
        cv::drawContours(drawing, contours, idx, cv::Scalar(0, 0, 255));
    }
    std::cout << "couter : " << contours.size() - 1 << std::endl;

    while (true)
    {
        cv::imshow("canny", canny);
        cv::imshow("drawing", drawing);
        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
