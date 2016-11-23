#! /bin/sh

all_indiv_count=243

all_indiv_iter=1

while [ $all_indiv_iter -le $all_indiv_count ]
do
#  echo "============================================="
  search_word=` cat all_indiv_func | head -$all_indiv_iter | tail -1`
  dummy=`grep -v _AMD kernel_[0-9]_func | grep \ $search_word\ $`
  if [ $? -eq 1 ]
  then
    echo $search_word
  fi
  all_indiv_iter=`expr $all_indiv_iter + 1`
#  echo "============================================="
done

