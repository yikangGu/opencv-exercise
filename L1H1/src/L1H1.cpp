#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

main()
{
    cv::Mat img;
    img = cv::imread("folder/fig1.jpg", 0);

    if (img.empty())
    {
        std::cout << "img is empty" << std::endl;
        return -1;
    }

    // loop over operator. there have 3 methods to do it. this is one of them.
    for (int x = 0; x < img.cols; x++)
    {
        for (int y = 0; y < img.rows; y++)
        {
            if (img.at<uchar>(y, x) != 2) img.at<uchar>(y, x) = 0;
            else img.at<uchar>(y, x) = 255;
        }
    }

    // create 5*5 kernel
    cv::Mat dilateImg, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(5, 5));

    // find contours : 
    //   RETR_LIST : to find all contours without establishing any hierarchical relationships.
    //   see : https://docs.opencv.org/3.4.1/d3/dc0/group__imgproc__shape.html#ga819779b9857cc2f8601e6526a3a5bc71
    //   CHAIN_APPROX_SIMPLE : to get less points from contours.
    //   see : https://docs.opencv.org/3.4.1/d3/dc0/group__imgproc__shape.html#ga4303f45752694956374734a03c54d5ff
    cv::dilate(img, dilateImg, kernel, cv::Point(-1, -1), 10);

    // find edge
    cv::Mat canny;
    cv::Canny(dilateImg, canny, 100, 200);

    // find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(dilateImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

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
        cv::imshow("drawing", drawing);
        if (cv::waitKey(1) == 27) break;
    }

    cv::destroyAllWindows();
    return 0;
}
