# CSC718 Final Project

## Project Description

Different impelementations of the traveling salesman problem to compare the performance of different implementations of sequential and parallel.

## Project Structure
- sequential
  - README.md
  - traveling_salesman.c
- openmp 
  - README.md
  - traveling_salesman_openmp.c
- mpi   
  - README.md
  - traveling_salesman_mpi.c

NOTE: Each of the above directories contains a README.md file that describes how to compile and run the program. 

## Test Files Expected Output

- The expected output for the test files is in this general format:
```
Enter Total Number of Cities:
Enter Cost Matrix

Enter 4 Elements in Row[1]

Enter 4 Elements in Row[2]

Enter 4 Elements in Row[3]

Enter 4 Elements in Row[4]

************************************
City Cost Matrix
************************************
  City (  0) :   0   2   9  10
  City (  1) :   1   0   6   4
  City (  2) :  15   7   0   8
  City (  3) :   6   3  12   0

************************************
Generating Paths / Weights
************************************
Path: 0 1 3 2 0 -> Total Weight: 33
 - Max Tours: 6.000000
************************************
  - New Best Tour
Path: 0 3 2 1 0 -> Total Weight: 30
  - New Best Tour
Path: 0 2 1 3 0 -> Total Weight: 26
  - New Best Tour
Path: 0 1 2 3 0 -> Total Weight: 22
  - New Best Tour
Path: 0 2 3 1 0 -> Total Weight: 21

************************************
All Possible Paths
************************************
General Info
  - Best Tour Cost: 21
************************************

************************************
Best Tour Paths
************************************
Path: 0 2 3 1 0 -> Total Weight: 21
************************************
Run Time: 0.000000 seconds  
```


## Generateing Test Files
- To generate test files, use the following command:
```
python generate_test_files.py number_of_cities > output_file
```

