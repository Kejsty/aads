## Usage: 

### Run all input files

To run all input files from directory **inputs** run: 

```
chmod +x test.sh
.\test.sh
```

This runs algorithm on every *.triangles from inputs, and compares result with *.result


### Compile implementation

```
clang++ -std=c++14 -o out main.cpp
./out inputs/some_input.triangles
```

The result than can be found in inputs/some_input.myresults

