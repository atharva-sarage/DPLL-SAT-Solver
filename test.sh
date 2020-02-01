#!/bin/bash
for filename in ./tests/jnh/*.cnf; do
    echo " $filename" >> out4
    #g++ solver.cpp --std=c++11 && ( time ./a.out < "$filename" ) >>out3
    time ./a2.out < "$filename" >>out4
    echo "---------------------------" >> out4
    echo "done, $filename "
done