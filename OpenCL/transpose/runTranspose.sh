#! /bin/bash

i=1
j=1

while [ $i -le 64 ]
  do
    j=1
    while [ $j -le 64 ]
      do
        ./a.out $i $j
        j=`expr $j + 1 `
      done
    i=`expr $i + 1 `
done
