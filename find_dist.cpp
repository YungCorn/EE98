#include "opencv2/calib3d/calib3d.hpp"

#include <cstdlib>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    if(argc == 12) {
        string fname = argv[1];
        int width, height;
        int Ax1, Ay1, Ax2, Ay2, Bx1, By1, Bx2, By2;
        
        width = atoi(argv[2]);
        height = atoi(argv[3]);
        Ax1 = atoi(argv[4]);
        Ay1 = atoi(argv[5]);
        Ax2 = atoi(argv[6]);
        Ay2 = atoi(argv[7]);
        Bx1 = atoi(argv[8]);
        By1 = atoi(argv[9]);
        Bx2 = atoi(argv[10]);
        By2 = atoi(argv[11]);

        FileStorage exts(fname, FileStorage::READ);
        Mat Q;
        exts["Q"] >> Q;

        float dispA = Ax1 - Ax2;
        float dispB = Bx1 - Bx2;

        Mat Disp = Mat::zeros(width, height, CV_32FC1);
        Disp.at<float>(Point(Ax1, Ay1)) = dispA;
        Disp.at<float>(Point(Bx1, By1)) = dispB;
        Mat Coords(width, height, CV_32FC3, Scalar(0, 0, 0));
        reprojectImageTo3D(Disp, Coords, Q);
        float xvalA = Coords.at<Vec3f>(Point(Ax1, Ay1))[0];
        float yvalA = Coords.at<Vec3f>(Point(Ax1, Ay1))[1];
        float zvalA = Coords.at<Vec3f>(Point(Ax1, Ay1))[2];
        float xvalB = Coords.at<Vec3f>(Point(Bx1, By1))[0];
        float yvalB = Coords.at<Vec3f>(Point(Bx1, By1))[1];
        float zvalB = Coords.at<Vec3f>(Point(Bx1, By1))[2];

        float distance = sqrt( pow((xvalA-xvalB), 2) + pow((yvalA-yvalB), 2) +
                                pow((zvalA-zvalB), 2) );
        cout << "Distance = " << distance << "mm\n\n";

        return 0;
    }
    else {
        cout << "Usage: \n";
        return 0;
    } 
}
