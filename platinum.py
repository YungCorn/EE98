#assure that you are on the same network as the raspberry pi's

import os

master_ip = '10.3.13.205'
slave_ip = '10.3.14.223'

def main():
    #os.system("" % slave _ip) # This needs to start the slave
    os.system("fab -H %s take_one_pic" % master_ip) # trigger the master to take pics
    os.system("fab -H %s get_master_pic" % master_ip) # get photo from master
    os.system("fab -H %s get_slave_pic" % slave_ip) # get photo from slave
    
    os.system("./stereo_rect intrinsics.yml extrinsics.yml user_pictures/left.png \
                user_pictures/right.png")
    
    os.system("python pic_display.py")
    
    with open("user_pictures_rect/coords.txt") as f:
        coords = f.readlines()
    c = coords[0].split()
    
    os.system("./find_dist extrinsics.yml 1680 1050 {} {} {} {} {} {} {} {}".format(\
                c[0], c[1], c[4], c[5], c[2], c[3], c[6], c[7]))


main()