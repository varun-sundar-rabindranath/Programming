#! /bin/bash

tillIp=$1

iterator=1

while [ $iterator -ne $tillIp ]
do
  ./a.out $iterator
  if [ $? -ne 0 ]
  then
    echo -e "\e[1;31mIncorrect output for size $iterator\e[0m"
  else
    echo "Correct output for size $iterator"
  fi
  iterator=`expr $iterator + 1`
done
