#! /usr/bin/python

import sys
import os

cdef public int add(char *s, int x, int y):
    cdef bytes py_str = s
    if(os.path.exists(py_str)):
        print "Path exists !! "
    else :
        print "Path does not exists !!"
    print (("%s %d %d - %d\n") % (py_str, x, y, x + y))
    return x + y
cdef public int sub(char *s, int x, int y):
    cdef bytes py_str = s
    if(os.path.exists(py_str)):
        print "Path exists !! "
    else :
        print "Path does not exists !!"
    print (("%s %d %d - %d\n") % (py_str, x, y, x - y))
    return x - y

