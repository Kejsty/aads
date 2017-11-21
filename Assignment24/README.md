## Usage: 

### Run all input files

To run all input files from directory **data** run: 

```
chmod +x test.sh
.\test.sh
```

This runs algorithm on every *.txt from inputs and compares result with *.result


### Compare O(n) and O(n^2) algorithms on inputs

To compare all input files from directory **data** run: 

```
chmod +x compare.sh
.\compare.sh
```

This runs both algorithms on every *.txt from inputs and compares their running time.


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

Ever since you have invented and built your own time-machine, you try to challenge yourself
with interesting games. In this instance, you want to buy shares in Silentpenguin, a company producing
targeted USB implants built to collect classified network data.
To make the game challenging, you limit yourself to two transaction in total: one
buying 10 000 shares, and one selling all shares (after you bought them; no using
the time machine for this, and no trading in the futures market).
Using your time-machine, you obtain the value of a share for Silentpenguin for
every day of the next couple of months. The data might look like the table on the
right.
Your goal is to maximize your earnings.
Devise an algorithm that, given the price development over time, tells you when
to buy and when to sell. One obvious solution is to test all Θ(n^2) pairs of days
(where n is the number of days). This problem can also be solved using a divide
and conquer approach, which should result in an asymptotically better bound.

*  Design a divide-and-conquery algorithm, describe it, and analyze its running time.
*  Implement both the brute-force version and your improved variant.



