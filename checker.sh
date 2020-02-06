#!/bin/bash
mkdir dump
#UUF175.753.100
#CBS_k3_n100_m403_b10
#UUF200.860.1000
g++ solver.cpp --std=c++11
for filename in ./tests/CBS_k3_n100_m403_b10/*.cnf; do   
    #./a.out < $filename &>> ./dump/out30
    time ./a.out < $filename
    # #time ./brute.out < "$filename" >> dump/out6
    echo "$filename"
    # echo "---------------------------" >>  dump/out7    
done
#diff -w correctAns.txt ./dump/out30
