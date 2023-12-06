# OpenMPI TSP

## Introduction
- This is a parallel TSP solver using OpenMPI.
- The solver finds the shortest path to visit all the cities in the given list, before returning to the starting city. To help save time during the search, the solver uses a lower bound to prune the search tree. If the lower bound of a partial path is greater than the length of the best path found so far, the solver will not explore that path further.

## How to compile
- To complie the solver, use the following command:
```
mpicc -o tsp_openmpi tsp.c
```

## How to run the solver with a given input file
- To run the solver, use the following command:
```
mpirun -machinefile machinefile -np <number_of_processors> ./tsp_openmpi <input_file>
```
