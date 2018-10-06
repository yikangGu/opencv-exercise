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

    cv::Mat dilateImg, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));
    cv::dilate(imgBG, dilateImg, kernel, cv::Point(-1, -1), 1);

    while (true)
    {
        cv::imshow("src", src);
        cv::imshow("dilateImg", dilateImg);
        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
