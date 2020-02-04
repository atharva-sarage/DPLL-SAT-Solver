#!/bin/bash
mkdir dump
#
g++ solver.cpp --std=c++11
for filename in ./tests/UUF125.538.100/*.cnf; do   
    #./a.out < $filename &>> ./dump/out21
    time ./a.out < $filename
    # #time ./brute.out < "$filename" >> dump/out6
    echo "$filename"
    # echo "---------------------------" >>  dump/out7    
done