#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
    VideoCapture capture("img/1.mp4");

    if (!capture.isOpened())
    {
        cout << "fail to open" << endl;
        return -1;
    }

    long maxFrame = capture.get(CV_CAP_PROP_FRAME_COUNT);
    cout<<"maxFrame : "<<maxFrame<<endl;

    Mat src;

    while (true)
    {
        capture >> src;

        if (src.empty())
        {
            cout << "video is over" << endl;
            return -1;
        }

        imshow("src", src);

        int cur = capture.get(CV_CAP_PROP_POS_FRAMES);
        if (cur == maxFrame)
            capture.set(CV_CAP_PROP_POS_FRAMES, 0);

        if (waitKey(0) == 27)
            break;
    }

    destroyAllWindows();
    capture.release();
    return 0;
}
