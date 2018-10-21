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

    // use the split method.
    // of couse, use the method that converting to hsv frame is also a good method.
    // base on this method, we know the following code is maybe unuseful for man who use the hsv frame.
    // but, even so, we can learn some thinking about low-level operation in opencv. 
    cv::Mat B, R;
    std::vector<cv::Mat> bgrChannels;
    cv::split(src, bgrChannels);
    B = bgrChannels.at(0);
    R = bgrChannels.at(2);

    cv::Mat redBinary;
    // do not ask me why r - b, cz i just try it out luckily.
    cv::Mat subRB = R - B;
    cv::threshold(subRB, redBinary, 100, 255, cv::THRESH_BINARY_INV);

    // backup the src for avoiding the src modified if the following code will modify the input image. 
    cv::Mat imgR = src.clone();
    cv::Mat imgBG = src.clone();

    // setTo is an operation from the core of opencv.
    // this is useful if you can operate the mat directly.
    // see also : 
    //   https://docs.opencv.org/3.4.1/d3/d63/classcv_1_1Mat.html#a0440e2a164c0b0d8462fb1e487be9876 
    imgR.setTo(0, redBinary);
    imgBG.setTo(0, imgR);

    // why the cols need multiply 3 times?
    // you need to think about this.(Ha, in fact, because the mat is an array.)
    for (int x = 0; x < imgBG.cols * 3; x++)
        for (int y = 0; y < imgBG.rows; y++)
            if (imgBG.at<uchar>(y, x) == 255)
                imgBG.at<uchar>(y, x) = 0;

    cv::Mat grayBG, grayR;
    cv::cvtColor(imgBG, grayBG, cv::COLOR_BGR2GRAY);
    cv::cvtColor(imgR, grayR, cv::COLOR_BGR2GRAY);

    cv::Mat erodeImg, kernel;
    kernel = cv::getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));
    cv::erode(grayBG, erodeImg, kernel, cv::Point(-1, -1), 1);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(erodeImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int couterBG = contours.size();
    cv::findContours(grayR, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    std::cout << "Objects : " << contours.size() + couterBG << std::endl;
    
    // here is too.
    cv::Mat subBR = B - R;
    cv::threshold(subBR, subBR, 250, 255, cv::THRESH_BINARY);
    cv::findContours(subBR, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    std::cout << "blue ball : " << contours.size() << std::endl;

    std::vector<cv::Point> leftBall = contours[1];
    std::vector<cv::Point> rightBall = contours[2];

    float _;
    cv::Point2f centerBallLeft;
    cv::Point2f centerBallRight;

    // you can use the minEnclosing rectangle method,
    // it doesn't matter, the result is the same in theory.
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
        cv::imshow("src", grayBG);
        if (cv::waitKey(1) == 27)
            break;
    }

    cv::destroyAllWindows();
    return 0;
}
