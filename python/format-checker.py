
''' Given a file, check for formatting errors '''

import os
import sys

help_string = '''

  format-checker.py :

    Given a filename as input, the program checks if the file/code is
    properly formatted.

    Example:
      format-checker.py filename.rkt
'''

if (len(sys.argv) != 2):
    print (help_string)
    exit(-1)

filename = sys.argv[1]

def check_format(line, filename, line_num):

    if (len(line) > 80):
        print ("LINE > 80 CHARACTERS")
        print (filename, " : ", line_num, line)

    for c in line:
        if (c == '\t'):
            print ("TABS")
            print (filename, " : ", line_num, line)


print ("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")

file_content = ""

line_num = 0;

with open(filename) as f:
    for line in f:
        check_format(line, filename, line_num)
        line_num++

print (file_content)

print ("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
