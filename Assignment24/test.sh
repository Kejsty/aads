#!/bin/sh
# Simple script to run all tests
#compile
clang++ -std=c++14 -o out solution.cpp
cd "data"
for x in *.result; do 
	t=$(echo $x | sed 's/\.result$//');
  	../out $t
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