# http://advanced-python.readthedocs.org/en/latest/fabric.html
# run with 'fab take_one_pic'
from fabric.api import(
    run,
    env,
    )
from fabric.context_managers import cd
from fabric.operations import get

env.password = 'raspberry' # password of master
env.user = 'pi'

def start_slave():
    #run with 'fab -H <IPP ADDRESS OF SLAVE> start_slave'
    with cd('~/Documents/python/'):
        run('sudo python take_one_pic.py')


def take_one_pic():
    # run with 'fab -H <IP ADDRESS OF MASTER> take_one_pic'
    with cd('~/Documents/stereo_cam_MASTER/'):
        run('sudo python take_one_pic.py')


def get_master_pic():
    # run with 'fab -H <IP ADDRESS OF MASTER> get_master_pic'
    get('~/Documents/stereo_cam_MASTER/user_pic/left.png','user_pictures_raw/left.png')


def get_slave_pic():
    # run with 'fab -H <IP ADDRESS OF SLAVE> get_master_pic'
    get('~/Documents/stereo_cam_SLAVE/user_pic/right.png','user_pictures_raw/right.png')

def kill_slave():
    # run with 'fab -H <IP ADDRESS OF MASTER>  kill_slave' (this is not a mistake, you
    # put the master's IP, not the slave's. The master kills the slave. It must be done
    # this way
    with cd('~/Documents/PythonTests/'):
        run('sudo python kill_slave.py')