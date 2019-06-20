#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>

cv::RotatedRect cv::minAreaRect( InputArray _points )
{
    cv::Mat hull;
    Point2f out[3];
    RotatedRect box;

    convexHull(_points, hull, true, true);

    if( hull.depth() != CV_32F )
    {
        Mat temp;
        hull.convertTo(temp, CV_32F);
        hull = temp;
    }

    int n = hull.checkVector(2);
    const Point2f* hpoints = hull.ptr<Point2f>();

    if( n > 2 )
    {
        rotatingCalipers( hpoints, n, CALIPERS_MINAREARECT, (float*)out );
        box.center.x = out[0].x + (out[1].x + out[2].x)*0.5f;
        box.center.y = out[0].y + (out[1].y + out[2].y)*0.5f;
        box.size.width = (float)std::sqrt((double)out[1].x*out[1].x + (double)out[1].y*out[1].y);
        box.size.height = (float)std::sqrt((double)out[2].x*out[2].x + (double)out[2].y*out[2].y);
        box.angle = (float)atan2( (double)out[1].y, (double)out[1].x );
    }
    else if( n == 2 )
    {
        box.center.x = (hpoints[0].x + hpoints[1].x)*0.5f;
        box.center.y = (hpoints[0].y + hpoints[1].y)*0.5f;
        double dx = hpoints[1].x - hpoints[0].x;
        double dy = hpoints[1].y - hpoints[0].y;
        box.size.width = (float)std::sqrt(dx*dx + dy*dy);
        box.size.height = 0;
        box.angle = (float)atan2( dy, dx );
    }
    else
    {
        if( n == 1 )
            box.center = hpoints[0];
    }

    box.angle = (float)(box.angle*180/CV_PI);
    return box;
}
