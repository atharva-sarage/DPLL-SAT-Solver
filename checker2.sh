#!/bin/bash
mkdir dump
for filename in ./tests/UUF125.538.100/*.cnf; do   
    ./brute3.out < $filename &>>./dump/out20
    # #time ./brute.out < "$filename" >> dump/out6
    # echo "---------------------------" >>  dump/out7    
    echo "$filename"
done