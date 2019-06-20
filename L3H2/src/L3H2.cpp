#include "L3H2.h"
#include "prev.hpp"

RotatedRect PREV_ARMOR;
Point2i PREV_PREC;

int main(int argc, char const *argv[])
{
    VideoCapture capture("img/1.mp4");

    if (!capture.isOpened())
    {
        cout << "fail to open" << endl;
        return -1;
    }

    long maxFrame = 816; // capture.get(CV_CAP_PROP_FRAME_COUNT); //
    cout << "maxFrame : " << maxFrame << endl;

    Mat src;
    int MODE = 0;

    while (true)
    {
        capture >> src;

        int cur = capture.get(CV_CAP_PROP_POS_FRAMES);
        if (cur >= maxFrame)
        {
            capture.set(CV_CAP_PROP_POS_FRAMES, 0);
            BB_STATE = DEACTIVATE_STATE;
        }
        if (cur % 12 != 0)
            continue;
        cout << " cur : " << cur << endl;

        if (src.empty())
        {
            cout << "video is over" << endl;
            return -1;
        }

        if (MODE == BIG_BUFF_STATE)
        {
            bool bbFlag = BBProcess(src);
        }

        imshow("src", src);

        if (waitKey(0) == 27)
            break;
    }

    destroyAllWindows();
    capture.release();
    return 0;
}

bool BBProcess(Mat src)
{
    Mat proc = src.clone();

    Mat B, R, G, subRG, bin;
    vector<Mat> bgr_channels;
    split(proc, bgr_channels);
    B = bgr_channels.at(0);
    G = bgr_channels.at(1);
    R = bgr_channels.at(2);
    subRG = R - G;
    threshold(subRG, bin, 40, 255, THRESH_BINARY);

    vector<vector<Point>> binary_contours;
    vector<Vec4i> binary_hierarchy;
    findContours(bin, binary_contours, binary_hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

    vector<Point2i> candidate_armor;
    vector<int> candidate_R;
    for (size_t idx = 0; idx < binary_hierarchy.size(); idx++)
    {
        if (binary_hierarchy[idx][2] == -1)
        {
            if (binary_hierarchy[idx][0] == -1 &&
                binary_hierarchy[idx][1] == -1)
            {
                int father_idx = binary_hierarchy[idx][3];
                candidate_armor.push_back(Point(idx, father_idx));
            }

            if (binary_hierarchy[idx][3] == -1)
            {
                candidate_R.push_back(idx);
            }
        }
    }

    int leaf_counter = 0;
    RotatedRect cur_armor_minRect;
    for (size_t armor_idx = 0; armor_idx < candidate_armor.size(); armor_idx++)
    {
        vector<Point> armor = binary_contours[candidate_armor[armor_idx].x];
        vector<Point> leaf = binary_contours[candidate_armor[armor_idx].y];
        if (IsArmor(armor) && IsLeaf(leaf))
        {
            leaf_counter += 1;
            cur_armor_minRect = minAreaRect(armor);

#ifdef DEBUG_LEAF
            cout << "[Find Leaf!]" << endl;
            drawContours(src, binary_contours, candidate_armor[armor_idx].x, Scalar(0, 0, 255));
#endif
        }
    }

    int R_counter = 0;
    for (size_t R_idx = 0; R_idx < candidate_R.size(); R_idx++)
    {
        int result = IsR(binary_contours[candidate_R[R_idx]]);
        if (result == 1)
        {
            R_counter += 1;
#ifdef DEBUG_R
            cout << "[Find R!]" << endl;
            drawContours(src, binary_contours, candidate_R[R_idx], Scalar(0, 0, 255));
#endif
        }
    }

    // state estimation
    if (leaf_counter == 0 && R_counter == 1 && DEACTIVATE_STATE > BB_STATE)
        BB_STATE = DEACTIVATE_STATE;
    else if (leaf_counter == 1 && ACTIVATING_STATE >= BB_STATE)
    {
        BB_STATE = ACTIVATING_STATE;
        if (PREV_ARMOR.center.x == 0)
            PREV_ARMOR = cur_armor_minRect;
        else
        {
            Point2i cur_prec = get2vsv(
                cur_armor_minRect.center.x,
                cur_armor_minRect.center.y,
                PREV_ARMOR.center.x,
                PREV_ARMOR.center.y);
#ifdef DEBUG_LEAF
            circle(src, cur_prec, 2, Scalar(0, 255, 0), CV_FILLED, CV_AA);
            circle(src, PREV_PREC, 2, Scalar(255, 0, 0), CV_FILLED, CV_AA);
            // circle(src, Point2i(cur_armor_minRect.center.x, cur_armor_minRect.center.y), 2, Scalar(0, 0, 255), CV_FILLED, CV_AA);
            PREV_PREC = cur_prec;
#endif
            PREV_ARMOR = cur_armor_minRect;
        }
    }
    else if (leaf_counter == 5 && FINISH_STATE > BB_STATE)
        BB_STATE = FINISH_STATE;
    else
    {
        cout << "[ERROR_STATE]" << endl;
        PREV_PREC = Point2i(0, 0);
        PREV_ARMOR.center.x = 0;
        PREV_ARMOR.center.y = 0;
    }
    cout << "[BB_STATE]: " << BB_STATE << endl;

    return false;
}

bool IsLeaf(vector<Point> candidate_leaf)
{
    // drawContours(src, binary_contours, idx, Scalar(0, 255, 0));

    vector<Point> hulls;
    convexHull(candidate_leaf, hulls, true);
    return true;
}

bool IsArmor(vector<Point> candidate_armor)
{
    // drawContours(src, binary_contours, idx, Scalar(0, 255, 0));

    vector<Point> hulls;
    convexHull(candidate_armor, hulls, true);
    return true;
}

int IsR(vector<Point> candidate_R)
{
    // drawContours(src, binary_contours, idx, Scalar(0, 255, 0));

    vector<Point> hulls;
    convexHull(candidate_R, hulls, true);
    return 1;
}
