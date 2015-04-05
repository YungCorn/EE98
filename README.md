# EE98
TEAM PLATINUM

##Compilation
To compile follow these directions:  
`> cd <cloned directory>`  
`> cmake .`  
`> make`

##Usage
###stereo_calib
The purpose of this program is to generate the stereo camera parameters. It
creates two files, `intrinsics.yml` and `extrinsics.yml` which contain the
data.

To run:  
`> ./stereo_calb -w <X> -h <Y> <pics.xml>`  
Where `<X>` is the number of checkerboard intersections along the x-axis,  
and `<Y>` is the number of checkerboard intersections along the y-axis,  
and `<pics.xml>` is an xml file that specifies the desired calibration photos
which should be used.

###find_dist
The purpose of this program is to find the real world distance between two
points in a stereo image.

To run:  
`> ./find_dist <extrinsics.yml> <width> <height> <L1x> <L1y> <R1x> <R1y> <L2x> <L2y> <R2x> <R2y>`  
Where `<extrinsics.yml>` is a yaml file that contains the extrinsic data
generated from stereo_calib,  
and `<width>` is the width of the images in pixels,  
and `<height>` is the height of the images in pixels,
and `<L1x>`is the x-coordinate of the first point in the left photo,  
and `<L1y>`is the y-coordinate of the first point in the left photo,  
and `<R1x>`is the x-coordinate of the first point in the right photo,  
and `<R1y>`is the y-coordinate of the first point in the right photo,  
and `<L2x>`is the x-coordinate of the second point in the left photo,  
and `<L2y>`is the y-coordinate of the second point in the left photo,  
and `<R2x>`is the x-coordinate of the second point in the right photo,  
and `<R2y>`is the y-coordinate of the second point in the right photo.

