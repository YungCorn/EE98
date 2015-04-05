#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cstdlib>
#include <iostream>

using namespace cv;
using namespace std;


/* This program rectifies a pair of stereo images.
 *
 * It takes in an instrinsic camera parameters yaml file, an extrinsic camera
 * parameters yaml file, the name of the left stereo image, and the name of the
 * right stereo image.
 *
 * It saves two images to the directory in which it was run:
 *      leftRect.png
 *      rightRect.png
 * These are the rectified versions of the images that were passed in
 */
int main(int argc, char** argv)
{
    if (argc == 5)
    {
        string intsName = argv[1];
        string extsName = argv[2];
        string leftName = argv[3];
        string rightName = argv[4];

        // Load the two images
        Mat leftImg = imread(leftName);
        Mat rightImg = imread(rightName);

        // Get image size
        Size imageSize = leftImg.size();
        if (imageSize != rightImg.size()) {
            cerr << "Error: Images are not the same size!\n";
            return 1;
        } 

        FileStorage ints(intsName, FileStorage::READ);
        FileStorage exts(extsName, FileStorage::READ);
        Mat cameraMatrix[2];
        Mat distCoeffs[2];
        Mat R1, R2, P1, P2;

        // Load all of the camera parameters from the yaml files
        ints["M1"] >> cameraMatrix[0];
        ints["M2"] >> cameraMatrix[1];
        ints["D1"] >> distCoeffs[0];
        ints["D2"] >> distCoeffs[1];
        exts["R1"] >> R1;
        exts["R2"] >> R2;
        exts["P1"] >> P1;
        exts["P2"] >> P2;

        // Compute the rectified images
        Mat rmap[2][2];
        Mat leftRect, rightRect;
        initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1,
            imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
        initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2,
            imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
        remap(leftImg, leftRect, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
        remap(rightImg, rightRect, rmap[1][0], rmap[1][1], CV_INTER_LINEAR);

        // Store the rectified images
        imwrite("leftRect.png", leftRect);
        imwrite("rightRect.png", rightRect);

        exts.release();
        ints.release();

        return 0;
    }
    else
    {
        cerr << "Usage:\n";
        return 1;
    }
}
