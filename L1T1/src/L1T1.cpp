#include <iostream>
#include <vector>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/core/core.hpp>

bool RectXCmp(const cv::RotatedRect &first, const cv::RotatedRect &second)
{
    if (first.center.x < second.center.x)
        return true;
    return false;
}

void Hole(const cv::Mat src, cv::Mat &dst, int mode = 0)
{
    cv::Size size = src.size();
    cv::Mat cut, temp;

    temp = cv::Mat::zeros(size.height + 2, size.width + 2, src.type());
    src.copyTo(temp(cv::Range(1, size.height + 1), cv::Range(1, size.width + 1)));
    cv::floodFill(temp, cv::Point(0, 0), cv::Scalar(255));
    temp(cv::Range(1, size.height + 1), cv::Range(1, size.width + 1)).copyTo(cut);

    if (mode == 0)
        dst = src | (~cut);
    else if (mode == 1)
        dst = ~cut;
}

int main(int argc, char const *argv[])
{
    cv::Mat src = cv::imread("img/1.png");

    if (src.empty())
    {
        std::cout << "img is empty" << std::endl;
        return -1;
    }

    cv::Mat kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));

    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

// black ball process
    cv::Mat ballcanny, hole;
    cv::Canny(gray, ballcanny, 127, 255);
    Hole(ballcanny, hole);
    cv::erode(hole, hole, kernel, cv::Point(-1, -1), 2);

    // find ipad proc
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(hole, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> candidate_rect;
    cv::RotatedRect rotate;
    for (size_t idx = 0; idx < contours.size(); idx++)
    {
        rotate = cv::minAreaRect(contours[idx]);
        double angle = abs(rotate.angle);

        if (50 < angle && angle < 70)
            candidate_rect.push_back(rotate.boundingRect());
    }

    if (candidate_rect.size() < 1)
    {
        std::cout << "no match" << std::endl;
        return -1;
    }

    // find ball proc
    cv::Mat ipad_roi = ballcanny(candidate_rect[0]);
    Hole(ipad_roi, ipad_roi, 1);

    cv::findContours(ipad_roi, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,
                     cv::Point(candidate_rect[0].x, candidate_rect[0].y));

    std::vector<cv::RotatedRect> candidate_ball;
    for (size_t idx = 0; idx < contours.size(); idx++)
    {
        rotate = cv::minAreaRect(contours[idx]);
        double angle = abs(rotate.angle);

        if (angle == 0)
        {
            cv::drawContours(src, contours, idx, cv::Scalar(0, 0, 255));
            candidate_ball.push_back(rotate);
        }
    }

    // sort and draw
    std::sort(candidate_ball.begin(), candidate_ball.end(), RectXCmp);
    for (size_t idx = 0; idx < candidate_ball.size() - 1; idx++)
    {
        // std::cout << " x[" << idx << "]: " << candidate_ball[idx].x << std::endl;
        cv::line(src,
                 cv::Point(candidate_ball[idx].center.x, candidate_ball[idx].center.y),
                 cv::Point(candidate_ball[idx + 1].center.x, candidate_ball[idx + 1].center.y), 
                 cv::Scalar(0, 0, 255));
    }

// find map process

    cv::Mat bin;
    cv::threshold(gray, bin, 127, 255, cv::THRESH_BINARY);
    cv::erode(bin, bin, kernel);

    // find map proc
    contours.clear();
    cv::findContours(bin, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    double maxContour = 0;
    int maxContour_idx = 0;
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (maxContour < area)
        {
            maxContour = area;
            maxContour_idx = i;
        }
    }
    cv::Rect maxContour_rect = cv::boundingRect(contours[maxContour_idx]);
    cv::Mat map_roi = gray(maxContour_rect);

    // show and draw
    cv::Mat map_canny;
    cv::Canny(map_roi, map_canny, 0, 2);
    cv::dilate(map_canny, map_canny, kernel);

    cv::Mat zeros = cv::Mat::zeros(src.rows + 2, src.cols + 2, CV_8UC1);
    int x = maxContour_rect.x;
    int y = maxContour_rect.y;
    map_canny.copyTo(zeros(maxContour_rect));
    cv::floodFill(src, zeros, cv::Point(x, y), cv::Scalar(255, 255, 255));

    while (true)
    {
        cv::imshow("src", src);
        cv::imshow("map_canny", map_canny);
        cv::imshow("ipad_roi", ipad_roi);

        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
