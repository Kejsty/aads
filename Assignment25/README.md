## Usage: 

### Run all input files

To run all input files from directory **tree** run: 

```
chmod +x test.sh
.\test.sh
```

This runs algorithm on every *.txt from inputs and compares result with *.result


### Compile implementation

```
clang++ -std=c++14 -o out solution.cpp
./out data/some_input
```

It lookups a file data/some_input.txt and solves this instance.
The result can be subsequently found in inputs/some_input.myresults

## Data

These input data were given by [Dr Peter Palfrader](https://www.palfrader.org/) for this assignment.

## Assignemnt

For a graph G = (V,E) an independent set S is a
subset of V such that no two vertices of S are connected by an edge
of E. 

In general, finding the largest independent set of a graph is a difficult
problem, but it is easier for trees. Design and implement an algorithm
that, given a tree T, finds (one) largest independent set of T.



