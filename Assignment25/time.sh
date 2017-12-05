#!/bin/sh
# Simple script to run all tests
#compile
clang++ -std=c++14 -o out solution.cpp

for ((i=10;i<=10000000;i*=10)); do
    python3 gen_tree.py $i >generated.tree
    ./out -time generated.tree
done
rm "out"
rm "generated.tree"