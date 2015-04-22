# assure that you are on the same network as the raspberry pis

import os

master_ip = '130.64.97.33'
slave_ip = '130.64.218.101'

def main():
    while(True):
        # Must start the slave separately in a different terminal, or do some smart way
        # to deal with the blocking.
        #os.system("fab -H %s take_one_pic" % master_ip) # trigger the master to take pics
        #os.system("fab -H %s get_master_pic" % master_ip) # get photo from master
        #os.system("fab -H %s get_slave_pic" % slave_ip) # get photo from slave
    
        # Rectifies the photos
        os.system("./OpenCV_components/stereo_rect OpenCV_components/intrinsics.yml\
                  OpenCV_components/extrinsics.yml user_pictures_raw/left.png \
                  user_pictures_raw/right.png")
    
        # This runs the GUI to display the pictures to the user for measurement selection
        os.system("python pic_display.py")
    
        # open up the file that pic_display wrote to
        # pass in the points to find_dist
        with open("user_pictures_rect/coords.txt") as f:
            coords = f.readlines()
        c = coords[0].split()
        os.system("./OpenCV_components/find_dist extrinsics.yml 1680 1050 {} {} {} {} {} {} {} {}".format(\
                    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]))
                    
        # display the result to the user           
        os.system("python result_display.py")

main()