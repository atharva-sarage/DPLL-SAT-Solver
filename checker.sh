#!/bin/bash
mkdir dump
#UUF175.753.100
#CBS_k3_n100_m403_b10
#UUF200.860.1000

g++ CS17BTECH11005-sat.cpp --std=c++11
for filename in $1/*.cnf; do   
    ./a.out < $filename >> ./dump/out2.txt
    time ./a.out < $filename
    echo "$filename"
done
#diff -w correctAns.txt ./dump/out2.txt
