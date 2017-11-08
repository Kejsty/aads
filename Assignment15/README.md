## Usage: 

### Run all input files

To run all input files from directory **inputs** run: 

```
chmod +x test.sh
.\test.sh
```

This runs algorithm on every *.triangles from inputs and compares result with *.result


### Compile implementation

```
clang++ -std=c++14 -o out main.cpp
./out inputs/some_input.triangles
```

The result can be subsequently found in inputs/some_input.myresults


## Assignemnt

You are given a polygon P as a list of vertices with
(x, y)-coordinates in counter-clockwise order, and you have an algorithm
that computes a constrained triangulation of the convex hull of
the point set and polygon edges. This algorithm provides you with an
(unordered) list of triangles, given as triples of (counter-clockwise) vertex
indices. Furthermore, this algorithm provides you with the (up to
three neighbors) for each triangle.

Come up with a time-optimal algorithm that marks each of the
triangles as being either inside or outside of the polygon.


## Idea

For each triangle, we can say it's outside of the polygon if the follows holds (this is possible only thank to the fact that triangles are given as triples of (counter-clockwise) vertex and the order of neighbors is not random!):

triangle (a,b,c) with neighbors (x,y,z) is ouside if and only if: 
*	(a,b) isn't the edge of polygon an z belongs to some outside triangle
*	(b,c) isn't the edge of polygon an x belongs to some outside triangle
*	(c,a) isn't the edge of polygon an y belongs to some outside triangle

We will refer the area ouside all triangles as "triangle" -1.
Thus, we can incrementally build the set of outside triangles, starting with set with "triangle" -1.


