To build Evilution, run the command:

	mpic++ -o <filename> main.cpp

To run Evilution, run the command:

	mpiexec -n <number of processors> ./cmake-build-debug/<filename>
	
Note: the number of processors must be square.