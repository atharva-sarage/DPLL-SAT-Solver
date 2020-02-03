#!/bin/bash
mkdir dump
for filename in ./tests/jnh/*.cnf; do   
    ./brute2.out < $filename &>>./dump/out12
    # #time ./brute.out < "$filename" >> dump/out6
    # echo "---------------------------" >>  dump/out7    
    sleep 1
done