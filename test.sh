#!/bin/bash
mkdir dump
for filename in ./tests/jnh/*.cnf; do
    echo " $filename" >> dump/out6
    g++ solver.cpp --std=c++11 && ( time ./a.out < "$filename" ) >>out6
    #time ./a.out < "$filename" >> dump/out6
    echo "---------------------------" >>  dump/out6
    echo "done, $filename "
    sleep 10
done