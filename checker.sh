#!/bin/bash
mkdir dump
g++ solver.cpp --std=c++11
for filename in ./tests/jnh/*.cnf; do   
    ./a.out < $filename &>> ./dump/out13
    # #time ./brute.out < "$filename" >> dump/out6
    echo "$filename"
    # echo "---------------------------" >>  dump/out7    
done