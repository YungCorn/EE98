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

// Primary method of calculating distance using reprojectImageTo3d
//---------------------------------------------------------------------------//

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
        cout << "Distance = " << distance << "mm\n\n";
        ofstream distance_file;
        distance_file.open("distance.txt");
        distance_file << distance;
        distance_file.close();

// Secondary method of calculating distance using triangulatePoints
//---------------------------------------------------------------------------//
//  for whatever reason, this method seems to be worse than the previous

        // Read in projection matrices from the extrinsic parameters file
        Mat P1, P2;
        exts["P1"] >> P1;
        exts["P2"] >> P2;

        // Create matrices of corresponding points
        Mat leftPoints = (Mat_<float>(2,2) << Ax1, Bx1, Ay1, By1);
        Mat rightPoints = (Mat_<float>(2,2) << Ax2, Bx2, Ay2, By2);

        // Find homogeneous coordinates of the two points
        Mat hPoints, pointsH(1, 2, CV_32FC4);
        triangulatePoints(P1, P2, leftPoints, rightPoints, hPoints);

        // Convert homogeneous coordinates into (x, y, z) coordinates
        pointsH.at<Vec4f>(0,0)[0] = hPoints.at<float>(0,0);
        pointsH.at<Vec4f>(0,0)[1] = hPoints.at<float>(1,0);
        pointsH.at<Vec4f>(0,0)[2] = hPoints.at<float>(2,0);
        pointsH.at<Vec4f>(0,0)[3] = hPoints.at<float>(3,0);
        pointsH.at<Vec4f>(0,1)[0] = hPoints.at<float>(0,1);
        pointsH.at<Vec4f>(0,1)[1] = hPoints.at<float>(1,1);
        pointsH.at<Vec4f>(0,1)[2] = hPoints.at<float>(2,1);
        pointsH.at<Vec4f>(0,1)[3] = hPoints.at<float>(3,1);
        Mat points3D;
        convertPointsFromHomogeneous(pointsH, points3D);

        // Use (x, y, z) coordinates to calculate distance
        float xval1 = points3D.at<Vec3f>(0, 0)[0];
        float yval1 = points3D.at<Vec3f>(0, 0)[1];
        float zval1 = points3D.at<Vec3f>(0, 0)[2];
        float xval2 = points3D.at<Vec3f>(0, 1)[0];
        float yval2 = points3D.at<Vec3f>(0, 1)[1];
        float zval2 = points3D.at<Vec3f>(0, 1)[2];
        float newDist = sqrt( pow((xval1-xval2), 2) + pow((yval1-yval2), 2) +
                                pow((zval1-zval2), 2) );

        // Output distance
        cout << "Alternate Distance = " << newDist << "mm\n\n";

//---------------------------------------------------------------------------//

        // Clean up
        exts.release();
        return 0;
    }
    else {
        cout << "Usage: \n";
        return 0;
    } 
}
