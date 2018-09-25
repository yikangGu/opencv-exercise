#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

main()
{
    Mat img;
    img = imread("folder/fig1.jpg");

    if (img.empty())
    {
        std::cout << "img is empty" << std::endl;
        return -1;
    }

    while (true)
    {
        imshow("img", img);
        if (waitKey(1) == 27)
            break;
    }

    destroyAllWindows();
    return 0;
}
