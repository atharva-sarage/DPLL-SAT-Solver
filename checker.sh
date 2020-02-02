#!/bin/bash
mkdir dump
for filename in ./tests/UUF125.538.100/*.cnf; do
    echo " $filename" >> dump/out6
    #g++ solver.cpp --std=c++11 && ( time ./a.out < "$filename" ) >>out6
    time ./brute.out < "$filename" >> dump/out6
    echo "---------------------------" >>  dump/out6
    echo "done, $filename "
done