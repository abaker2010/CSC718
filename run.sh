cd ./openmpi

mpirun -machinefile machinefile.dsu -np 2 ./tsp_openmpi ../test_files/big_test_25
mpirun -machinefile machinefile.dsu -np 1 ./tsp_openmpi ../test_files/big_test_25

cd ../openmp

./tsp_openmp < ../test_files/big_test_25

cd ../sequential

./tsp < ../test_files/big_test_25