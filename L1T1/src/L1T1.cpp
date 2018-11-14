#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

void countHist(cv::Mat src, std::vector<int> &counter, int mode = 0)
{
    if (mode == 0)
    {
        for (int x = 0; x < src.cols; x++)
        {
            counter.push_back(cv::countNonZero(src.col(x)));
        }
    }
    else if (mode == 1)
    {
        for (int y = 0; y < src.rows; y++)
        {
            counter.push_back(cv::countNonZero(src.row(y)));
        }
    }
}

void getPeeks(std::vector<int> counter, std::vector<int> &peeks, int thresh)
{
    int flagStart = 0;
    int flagEnd = 0;

    for (int i = 0; i < counter.size() - 1; i++)
    {
        if (counter[i] > thresh)
        {
            flagStart = 1;
            if (flagEnd == 0)
            {
                peeks.push_back(i);
            }
            flagEnd = 1;
        }
        else
        {
            if (flagStart == 1)
            {
                peeks.push_back(i);
                flagStart = 0;
            }
            flagEnd = 0;
        }
    }
    peeks.push_back(counter.size());
}

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

    // cv::Mat blackBinary;
    // cv::inRange(src, cv::Scalar(82, 72, 65), cv::Scalar(83, 73, 66), blackBinary);

    cv::Mat ballsrc = src(cv::Range(411, src.rows), cv::Range(700, src.cols));

    cv::Mat ballgray;
    cv::cvtColor(ballsrc, ballgray, CV_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(ballgray, binary, 127, 255, CV_THRESH_BINARY);

    cv::floodFill(binary, cv::Point(0, 0), cv::Scalar(0, 0, 0));
    cv::floodFill(binary, cv::Point(200, 0), cv::Scalar(0, 0, 0));

    std::vector<int> hist;
    countHist(binary, hist, 1);

    std::vector<int> hpeeks;
    getPeeks(hist, hpeeks, 20);

    std::vector<int> vist;
    countHist(binary(cv::Range(hpeeks[0], hpeeks[1]), cv::Range(0, binary.cols)), vist, 0);

    std::vector<int> vpeeks;
    getPeeks(vist, vpeeks, 20);

    cv::Mat roiSrc = binary(cv::Range(hpeeks[0], hpeeks[1]), cv::Range(vpeeks[0], vpeeks[1]));

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(roiSrc, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    float _;
    cv::Point2f fristBall, secondBall;
    int order[6] = {2, 1, 4, 3, 6, 5};

    for (size_t idx = 0; idx < 5; idx++)
    {
        cv::minEnclosingCircle(contours[order[idx]], fristBall, _);
        cv::minEnclosingCircle(contours[order[idx + 1]], secondBall, _);
        cv::line(
            src,
            cv::Point((int)fristBall.x + vpeeks[0] + 700, (int)fristBall.y + hpeeks[0] + 411),
            cv::Point((int)secondBall.x + vpeeks[0] + 700, (int)secondBall.y + hpeeks[0] + 411),
            cv::Scalar(0, 0, 255));
    }

    // find map process
    cv::Mat mapsrc = src(cv::Range(0, 410), cv::Range(700, src.cols));

    cv::Mat gray;
    cv::cvtColor(mapsrc, gray, CV_BGR2GRAY);

    cv::Mat canny;
    cv::Canny(gray, canny, 0, 2);

    cv::Mat dilate, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(2, 2));
    cv::dilate(canny, dilate, kernel, cv::Point(-1, -1), 1);

    cv::Mat zeros = cv::Mat::zeros(src.rows + 2, src.cols + 2, CV_8UC1);
    dilate.copyTo(zeros(cv::Rect(cv::Point(699, 0), mapsrc.size())));

    cv::floodFill(src, zeros, cv::Point(750, 50), cv::Scalar(255, 255, 255));

    while (true)
    {
        cv::imshow("roiSrc", roiSrc);
        cv::imshow("src", src);

        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
