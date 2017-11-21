#!/bin/sh
# Simple script to run all tests
#compile
clang++ -std=c++14 -o out solution.cpp
cd "data"
for x in *.result; do 
	t=$(echo $x | sed 's/\.result$//');
	echo "Input: $x"
  	../out $t "compare"
done
cd ..
rm "out"