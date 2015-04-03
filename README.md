# EE98
TEAM PLATINUM

##Compilation
To compile follow these directions:\n
`> cd <cloned directory>`\n
`> cmake .`\n
`> make`

##Usage
###stereo_calib
The purpose of this program is to generate the stereo camera parameters. It
creates two files, `intrinsics.yml` and `extrinsics.yml` which contain the
data.

To run:\n
`> ./stereo_calb -w <X> -h <Y> <pics.xml>`\n
Where `<X>` is the number of checkerboard intersections on the x-axis,\n
and `<Y>` is the number of checkerboard intersections in the y-axis,\n
and `<pics.xml>` is an xml file that specifies the desired calibration photos
which should be used.

###find_dist
The purpose of this program is to find the real world distance between two
points in a stereo image.

To run:\n
`> ./find_dist <extrinsics.yml> <L1x> <L1y> <R1x> <R1y> <L2x> <L2y> <R2x> <R2y>`\n
Where `<extrinsics.yml>` is a yaml file that contains the extrinsic data\n
generated from stereo_calib,
and `<L1x>`is the x-coordinate of the first point in the left photo\n
and `<L1y>`is the y-coordinate of the first point in the left photo\n
and `<R1x>`is the x-coordinate of the first point in the right photo\n
and `<R1y>`is the y-coordinate of the first point in the right photo\n
and `<L2x>`is the x-coordinate of the second point in the left photo\n
and `<L2y>`is the y-coordinate of the second point in the left photo\n
and `<R2x>`is the x-coordinate of the second point in the right photo\n
and `<R2y>`is the y-coordinate of the second point in the right photo\n

