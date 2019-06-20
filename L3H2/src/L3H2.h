#define DEBUG

#ifdef DEBUG
    #define DEBUG_LEAF
    // #define DEBUG_R
#endif

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

extern int BB_STATE = 0;

enum PROG_STATE
{
    BIG_BUFF_STATE = 0
};

enum BB_STATE
{
    ERROR_STATE,
    DEACTIVATE_STATE,
    ACTIVATING_STATE,
    FINISH_STATE
};


bool BBProcess(Mat src);
bool IsLeaf(vector<Point> candidate_leaf);
bool IsArmor(vector<Point> candidate_armor);
int IsR(vector<Point> candidate_R);
