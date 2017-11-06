## Usage

Python 3 is needed to run this code.

```
python matrix.py
```


## Assignment

You are given an array A of numbers A[1], A[2], . . . , A[n]. We want to fill in a square matrix
B such that B[i, j] for i < j contains the sum of all values from A[i] to A[j], inclusive, i.e., A[i] + A[i + 1] . . . + A[j]. (The contents of the remaining cells of B are undefined.)

The file matrix.py contains one basic approach in O(n^3) and two implementations in O(n^2). One of O(n^2) implementations tries to decrease the number of potential cash misses.