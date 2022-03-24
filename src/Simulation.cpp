#include <mpi.h>
#include "Simulation.h"
#include "Board.h"

using namespace std; 

// Constructor for a new simulation
Simulation::Simulation(int rank, int size, std::string configFilePath = "")
{
	myBoard = new Board(rank, size, configFilePath);
	myRank = rank;
	numProcessors = size;
	configPath = configFilePath;

	// TODO: config file processing

}

// Runs the full simulation for the specified number of "days"
void Simulation::RunSimulation(int daysToRun)
{
	for (int i = 0; i < daysToRun; ++i)
	{
		// TODO: New day begins (maybe .newDay function)

		for (int j = 1; j < 100; j++) {
			myBoard->timePassing(j);
			MPI_Barrier(MCW);
		}
	}
	
}

// Package up relevant information to send to a single process, 
// so information can be aggregated and outputted
void Simulation::GatherSimulationInfo()
{
	
}

// Output all relevant information into a .csv file for analysis
void Simulation::OutputSimulationResults()
{

}
