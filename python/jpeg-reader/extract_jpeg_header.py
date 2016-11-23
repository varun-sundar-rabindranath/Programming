import exifread
import os

filename = "./cannon_sample_1.jpg"

f = open(filename,'rb')

# Get exiftags
tags = exifread.process_file(f)

#print "Num tags -> %d " % len(tags)
#for tag in tags.keys():
#    print "Key : %s | Value : %s" %(tag, tags[tag])

# Write out thumbnail as a jpg
#thumbnail_data = tags["JPEGThumbnail"]
#f_out = open("./thumbnail.jpg", 'w+')
#f_out.write(thumbnail_data)
#f_out.close()

# Divide the file into 4k chunks !!
file_size = os.path.getsize(filename)

bin_str = "a"
while (bin_str != ""):
    bin_str = f.read(4096)
    # Make it a file and process it with exifread
    
    test_file = open("./test.jpg", 'wb+')
    test_file.write(bin_str)
    test_file.close()

    jpg_file = open("./test.jpg", 'rb')
    jpg_tags = exifread.process_file(jpg_file)
    print(jpg_tags)
    jpg_file.close()


photo_taken_time = tags["EXIF DateTimeDigitized"]
photo_taken_time_string = str(photo_taken_time)
print photo_taken_time_string
if '2016:0' in photo_taken_time_string :
    print ("Its a image start")

f.close()
