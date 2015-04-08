from Tkinter import *
from tkFileDialog import askopenfilename
from time import sleep
import Image
import ImageTk
import math
import os
import sys

# add in the zoom box before point selection

pic_width = 1680
pic_height = 1050
scaling_factor = 0.7
canvas_width = int(pic_width * scaling_factor)
canvas_height = int(pic_height * scaling_factor)

left_pic = "leftRect.png"
right_pic = "rightRect.png"
root = Tk()
canvas = Canvas(root, width=canvas_width, height=canvas_height)
canvas.pack()

image_list = []
    
def display_image():
    canvas.bind("<Button 1>",click_handler) # sets up event handler for clicking
    
    image = Image.open(left_pic)
    image = image.resize((canvas_width, canvas_height), Image.ANTIALIAS)
    img = ImageTk.PhotoImage(image)
    image_list.append(img)
    canvas.create_image(0,0,image=img,anchor="nw", tags="left_image")    
    
    with open("user_pictures_rect/coords.txt") as f:
        coords = f.readlines()
    c = coords[0].split()
    print c
    
    canvas.create_rectangle(int(c[0])*scaling_factor-5,
                            int(c[1])*scaling_factor-5,
                            int(c[0])*scaling_factor+5,
                            int(c[1])*scaling_factor+5,\
                           fill="green")
    canvas.create_rectangle(int(c[2])*scaling_factor-5,
                            int(c[3])*scaling_factor-5,
                            int(c[2])*scaling_factor+5,
                            int(c[3])*scaling_factor+5,\
                           fill="green")
    canvas.create_line( int(c[0])*scaling_factor,
                        int(c[1])*scaling_factor,
                        int(c[2])*scaling_factor,
                        int(c[3])*scaling_factor,
                        fill="red", tags="line")
    
    with open("distance.txt") as dist_file:
        distance = dist_file.readlines()[0]

    text_x = int(c[0])*scaling_factor
    text_y = int(c[1])*scaling_factor
    if(text_y) > canvas_height - 200:
        add_y = -50
    else:
        add_y = 50
    if(text_x) > canvas_width - 200 :
        add_x = -200
    else:
        add_x = 0
    text_x = text_x + add_x
    text_y = text_y + add_y
    canvas.create_text(text_x, text_y, font=("Purisa",25),\
                        fill="white", text= "Distance = %s mm"%distance)
    
    
    canvas.create_text(150, 30, font=("Purisa",25),\
                        fill="white", text= "Take New Picture")
    

def click_handler(event):    
    if( (event.x < 400) and (event.y < (100))):
        sys.exit()

    

display_image()
root.mainloop() # this is what actually runs the tkinter stuff
