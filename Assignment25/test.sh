#!/bin/sh
# Simple script to run all tests
#compile
clang++ -std=c++14 -o out solution.cpp
cd "trees"
for x in *.tree; do 
	t=$(echo $x | sed 's/\.tree$//');
  	../out -test $x
	if cmp -s "$t.myresults" "$t.result"
	then
	   echo "The results for $t matches"
	else
	   echo "The results for $t are different"
	fi
	rm "$t.myresults"
done
cd ..
rm "out"