#!/bin/bash
mkdir dump
#
g++ solver.cpp --std=c++11
for filename in ./tests/CBS_k3_n100_m403_b10/*.cnf; do   
    ./a.out < $filename &>> ./dump/out23
    #time ./a.out < $filename
    # #time ./brute.out < "$filename" >> dump/out6
    echo "$filename"
    # echo "---------------------------" >>  dump/out7    
done