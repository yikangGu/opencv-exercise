#include <vector>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Point2i get2vsv(int cur_x, int cur_y, int prev_x, int prev_y){
    return Point2i(2*cur_x-prev_x, 2*cur_y-prev_y);
}
