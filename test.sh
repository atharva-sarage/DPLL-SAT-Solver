#!/bin/bash
mkdir dump
for filename in ./tests/jnh/*.cnf; do
    echo " $filename" >> dump/out5
    #g++ solver.cpp --std=c++11 && ( time ./a.out < "$filename" ) >>out3
    time ./a2.out < "$filename" >> dump/out5
    echo "---------------------------" >>  dump/out5
    echo "done, $filename "
done