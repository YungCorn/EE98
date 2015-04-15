#include "opencv2/calib3d/calib3d.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

/* This program calculates the real-world distance between two points in a pair
 * of rectified stereo images.
 *
 * It takes in an extrinsic camera parameters yaml file, the width of the
 * images in pixels, and the height of the images in pixels. Then it takes in
 * all of the point information in this order:
 *      Left image, point 1, x-coordinate
 *      Left image, point 1, y-coordinate
 *      Right image, point 1, x-coordinate
 *      Right image, point 1, y-coordinate
 *      Left image, point 2, x-coordinate
 *      Left image, point 2, y-coordinate
 *      Right image, point 2, x-coordinate
 *      Right image, point 2, y-coordinate
 * 
 * It prints the real-world distance between the two points to the console
 */
int main(int argc, char** argv)
{
    if(argc == 12) {
        string fname = argv[1];
        int width, height;
        int Ax1, Ay1, Ax2, Ay2, Bx1, By1, Bx2, By2;
        
        // Take in all arguments
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

        // Read in 'Q' matrix from the extrinsic paramenters file
        FileStorage exts(fname, FileStorage::READ);
        Mat Q;
        exts["Q"] >> Q;

        // Calculate disparity between the given points
        float dispA = Ax1 - Ax2;
        float dispB = Bx1 - Bx2;

        // Find real-world (x,y,z) coordinates of the two points
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

        // Use (x,y,z) coordinates to calculate distance
        float distance = sqrt( pow((xvalA-xvalB), 2) + pow((yvalA-yvalB), 2) +
                                pow((zvalA-zvalB), 2) );

        // Output distance
        ofstream distance_file;
        distance_file.open("distance.txt");
        cout << "Distance = " << distance << "mm\n\n";
        distance_file << distance;
        distance_file.close();

        return 0;
    }
    else {
        cout << "Usage: \n";
        return 0;
    } 
}
