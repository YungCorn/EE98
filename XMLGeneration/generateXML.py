def generate():
    f = open("XMLGeneration/calibPics.xml", 'w')
    
    start = ("<?xml version=\"1.0\"?>\
             \n<opencv_storage>\
             \n<imagelist>\n")
    
    f.write(start)
    
    num_pairs = 50
    
    for num in range(num_pairs):
        line_left = ("\"raspPhotos/4_4_calib_photos/pics_4_4_left/left%d.png\"\n"%num)
        line_right =("\"raspPhotos/4_4_calib_photos/pics_4_4_right/right%d.png\"\n"%num)
        f.write(line_left)
        f.write(line_right)
        
    end = ("</imagelist>\
            \n</opencv_storage>")
    f.write(end)
    
generate()