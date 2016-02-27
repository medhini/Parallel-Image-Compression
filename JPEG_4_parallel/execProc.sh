#!/bin/sh
rm info.txt
g++ compression.cpp `pkg-config opencv --cflags --libs` -std=c++11 -fopenmp
a=80
while [ "$a" -lt 100 ]    # this is loop1
do
  ./a.out "$a"
  a=`expr $a + 1`
done
g++ calculateTime.cpp
./a.out
