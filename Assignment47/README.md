## Usage: 

### Compile

To compile the program, use:

```
clang++ -std=c++14 -o program read.cpp 
```

The program has two functionalities (for both Assignment 47 & 48):

For 47:
```
./program <input> <output>
example:
./out stvorec.bmp stvorec.tree
```


For 48:
```
./program union/intersect <input1> <input2> <output>
example:
./out intersect image.tree stvorec.tree intersect.txt
```

Some outputs can be found in the directory, namely: 
-    stvorec.tree: serialized quadtree for stvorec.bmp
-    image.tree: serialized quadtree for image.bmp
-    intersect.txt the result of intersection of stvorec.tree and image.tree
-    union.txt the result of union of stvorec.tree and image.tree


## Assignment

Assignment 47: Write a small helper tool that takes the filename of a black and white image as the first argument
and prints a serialized quadtree to the file given by the second argument.
Use a quadtree with as many levels as necessary to represent the image. Cells outside of the image are empty.
The tool that converts images to quadtree should take either png or BMP format. Use a library to load these
file formats; there is no need to parse them yourselves.

Assignment 48: The program implementing union and intersect should take 4 arguments. The first would
be intersect or union, then two filenames of serialized quadtrees, and finally the filename for the output
file.

