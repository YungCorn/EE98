#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cstdlib>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    string intsName = argv[1];
    string extsName = argv[2];
    string leftName = argv[3];
    string rightName = argv[4];

    Mat leftImg = imread(leftName);
    Mat rightImg = imread(rightName);

    Size imageSize = leftImg.size();
    //if (imageSize 

    FileStorage ints(intsName, FileStorage::READ);
    FileStorage exts(extsName, FileStorage::READ);
    Mat cameraMatrix[2];
    Mat distCoeffs[2];
    Mat R1, R2, P1, P2;

    ints["M1"] >> cameraMatrix[0];
    ints["M2"] >> cameraMatrix[1];
    ints["D1"] >> distCoeffs[0];
    ints["D2"] >> distCoeffs[1];
    exts["R1"] >> R1;
    exts["R2"] >> R2;
    exts["P1"] >> P1;
    exts["P2"] >> P2;

    Mat rmap[2][2];
    initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize,
        CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize,
        CV_16SC2, rmap[1][0], rmap[1][1]);

    Mat leftRect, rightRect;
    remap(leftImg, leftRect, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
    remap(rightImg, rightRect, rmap[1][0], rmap[1][1], CV_INTER_LINEAR);
    imwrite("leftRect.png", leftRect);
    imwrite("rightRect.png", rightRect);
}
