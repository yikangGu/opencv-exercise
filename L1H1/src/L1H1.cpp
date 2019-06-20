#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

main()
{
    cv::Mat img;
    img = cv::imread("img/1.jpg", 0);

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
            if (img.at<uchar>(y, x) != 2) img.at<uchar>(y, x) = 255;
            else img.at<uchar>(y, x) = 0;
        }
    }

    // create 3*3 kernel
    cv::Mat kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));

    // 
    cv::Mat erode, dilate;
    cv::erode(img, erode, kernel, cv::Point(-1,-1), 1);
    cv::dilate(erode, dilate, kernel, cv::Point(-1,-1), 1);

    //
    cv::Mat distance(dilate.size(), CV_32FC1);
    
    //
    cv::distanceTransform(dilate, distance, CV_DIST_L2, 5);
    
    // 
    cv::normalize(distance, distance, 0, 255, CV_MINMAX);
    distance.convertTo(distance, dilate.type());
    cv::threshold(distance, distance, 127, 255, CV_THRESH_BINARY);

    // find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(distance, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    // convect grey to bgr for drawing color in the image.
	cv::Mat drawing;
    cv::cvtColor(dilate, drawing, cv::COLOR_GRAY2BGR);

    // contours.size() -1  is means that abandon the biggest one(the last one)
    for (size_t idx = 0; idx < contours.size(); idx++)
    {
        cv::drawContours(drawing, contours, idx, cv::Scalar(255, 0, 0), -1);
    }
    std::cout << "counter : " << contours.size() << std::endl;

    while (true)
    {
        cv::imshow("drawing", drawing);

        if (cv::waitKey(1) == 27) break;
    }

    cv::destroyAllWindows();
    return 0;
}
