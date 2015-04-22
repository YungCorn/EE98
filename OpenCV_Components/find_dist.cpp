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
 *      Left image, point 2, x-coordinate
 *      Left image, point 2, y-coordinate
 *      Right image, point 1, x-coordinate
 *      Right image, point 1, y-coordinate
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
        int Lx1, Ly1, Lx2, Ly2, Rx1, Ry1, Rx2, Ry2;
        
        // Take in all arguments
        width = atoi(argv[2]);
        height = atoi(argv[3]);
        Lx1 = atoi(argv[4]);
        Ly1 = atoi(argv[5]);
        Lx2 = atoi(argv[6]);
        Ly2 = atoi(argv[7]);
        Rx1 = atoi(argv[8]);
        Ry1 = atoi(argv[9]);
        Rx2 = atoi(argv[10]);
        Ry2 = atoi(argv[11]);

// Primary method of calculating distance using reprojectImageTo3d
//---------------------------------------------------------------------------//

        // Read in 'Q' matrix from the extrinsic paramenters file
        FileStorage exts(fname, FileStorage::READ);
        Mat Q;
        exts["Q"] >> Q;

        // Calculate disparity between the given points
        float dispA = Lx1 - Rx1;
        float dispB = Lx2 - Rx2;

        // Find real-world (x,y,z) coordinates of the two points
        Mat Disp = Mat::zeros(width, height, CV_32FC1);
        Disp.at<float>(Point(Lx1, Ly1)) = dispA;
        Disp.at<float>(Point(Lx2, Ly2)) = dispB;
        Mat Coords(width, height, CV_32FC3, Scalar(0, 0, 0));
        reprojectImageTo3D(Disp, Coords, Q);
        float xval1 = Coords.at<Vec3f>(Point(Lx1, Ly1))[0];
        float yval1 = Coords.at<Vec3f>(Point(Lx1, Ly1))[1];
        float zval1 = Coords.at<Vec3f>(Point(Lx1, Ly1))[2];
        float xval2 = Coords.at<Vec3f>(Point(Lx2, Ly2))[0];
        float yval2 = Coords.at<Vec3f>(Point(Lx2, Ly2))[1];
        float zval2 = Coords.at<Vec3f>(Point(Lx2, Ly2))[2];

        // Use (x,y,z) coordinates to calculate distance
        float distance = sqrt( pow((xval1-xval2), 2) + pow((yval1-yval2), 2) +
                                pow((zval1-zval2), 2) );

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
        Mat leftPoints = (Mat_<float>(2,2) << Lx1, Lx2, Ly1, Ly2);
        Mat rightPoints = (Mat_<float>(2,2) << Rx1, Rx2, Ry1, Ry2);

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
        xval1 = points3D.at<Vec3f>(0, 0)[0];
        yval1 = points3D.at<Vec3f>(0, 0)[1];
        zval1 = points3D.at<Vec3f>(0, 0)[2];
        xval2 = points3D.at<Vec3f>(0, 1)[0];
        yval2 = points3D.at<Vec3f>(0, 1)[1];
        zval2 = points3D.at<Vec3f>(0, 1)[2];
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
