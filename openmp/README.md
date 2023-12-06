# OpenMP TSP 

## Introduction
- This is an OpenMP TSP solver.
- The solver finds the shortest path to visit all the cities in the given list, before returning to the starting city. To help save time during the search, the solver uses a lower bound to prune the search tree. If the lower bound of a partial path is greater than the length of the best path found so far, the solver will not explore that path further.

## How to compile
- To complie the solver, use the following command:
On Mac:
```
/opt/homebrew/opt/llvm/bin/clang -g -fopenmp -L/opt/homebrew/opt/llvm/lib traveling_salesman_openmp.c -o tsp_openmp
```

On Linux:
```
gcc -fopenmp -o tsp_openmp traveling_salesman_openmp.c 
```

## How to run the solver with a given input file
- To run the solver, use the following command:
```
./tsp_openmp < <input_file>
```