#include <iostream>
#include <vector>
#include <array>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

vector<Point> PRES;
int PREPOINTS = 5;
int FITTIMES = 2;

int getPredictX(vector<Point> preious)
{
    return (2 * preious[preious.size() - 1].x - preious[preious.size() - 2].x);
}

bool curveFit(vector<Point> points, int xn, Mat &A)
{
    int rows = points.size();
    xn = xn + 1;

    Mat X = Mat::zeros(rows, xn, CV_64FC1);
    Mat Y = Mat::zeros(rows, 1, CV_64FC1);

    for (int i = 0; i < rows; ++i)
    {
        Y.at<double>(i, 0) = points[i].y;
    }
    // cout << "Y : " << Y << endl;

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < xn; ++j)
        {
            X.at<double>(i, j) = pow(points[i].x, j);
        }
    }

    if (Y.empty() || X.empty())
        return false;

    A = (X.t() * X).inv() * X.t() * Y;

    if (A.empty())
        return false;
    return true;
}

bool isSquare(Rect rect, float thresh)
{
    return (rect.width > rect.height * (1 - thresh)) &&
           (rect.width < rect.height * (1 + thresh));
}

int main(int argc, char const *argv[])
{
    VideoCapture capture("folder/1.mp4");

    if (!capture.isOpened())
    {
        cout << "fail to open" << endl;
        return -1;
    }

    long maxFrame = capture.get(CV_CAP_PROP_FRAME_COUNT);

    Mat src, kernel;
    kernel = getStructuringElement(CV_SHAPE_RECT, cv::Size(3, 3));

    Scalar minG(35, 43, 46);
    Scalar maxG(77, 255, 255);

    capture.set(CV_CAP_PROP_POS_FRAMES, 30);

    int cur;
    Mat hsv, g;
    vector<vector<Point>> contours;

    Mat drawing;
    Rect rect;

    Mat params;
    Point2d preP, curP;
    preP.x = 0;
    preP.y = 0;
    int curPX;

    while (true)
    {
        cur = capture.get(CV_CAP_PROP_POS_FRAMES);
        cout << "cur : " << cur << endl;
        capture >> src;

        if (src.empty())
        {
            cout << "video is over" << endl;
            return -1;
        }

        cvtColor(src, hsv, COLOR_BGR2HSV);
        inRange(hsv, minG, maxG, g);

        erode(g, g, kernel, Point(-1, -1), 3);
        dilate(g, g, kernel, Point(-1, -1), 3);

        findContours(g, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        cvtColor(g, drawing, cv::COLOR_GRAY2BGR);

        for (size_t idx = 0; idx < contours.size(); idx++)
        {
            rect = boundingRect(contours[idx]);
            if (isSquare(rect, 0.5))
            {
                rectangle(drawing, rect, Scalar(0, 255, 0), 1);
                // if (cur % 3 == 0)
                // {
                PRES.push_back(Point(rect.x + (int)(rect.width / 2),
                                     rect.y + (int)(rect.height / 2)));
                // }
            }
        }

        if (PRES.size() == PREPOINTS)
        {
            curveFit(PRES, FITTIMES, params);
            // cout << "params : " << params << endl;

            curP.x = getPredictX(PRES);
            curP.y = 0;
            for (int j = 0; j < FITTIMES + 1; ++j)
            {
                curP.y += params.at<double>(j, 0) * pow(curP.x, j);
            }
            circle(drawing, curP, 2, Scalar(0, 0, 255), CV_FILLED, CV_AA);
            circle(drawing, preP, 2, Scalar(255, 0, 0), CV_FILLED, CV_AA);
            preP = curP;

            for (int i = 0; i < PRES.size(); ++i)
            {
                circle(drawing, PRES[i], 2, Scalar(0, 255, 0), CV_FILLED, CV_AA);
            }

            PRES.erase(PRES.begin());
        }

        imshow("drawing", drawing);

        if (cur == maxFrame - 1)
        {
            capture.set(CV_CAP_PROP_POS_FRAMES, 0);
            // cout << "set 0" << endl;
        }

        if (waitKey(0) == 27)
            break;
    }

    cv::destroyAllWindows();
    capture.release();
    return 0;
}