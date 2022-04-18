#include <mpi.h>
#include "Simulation.h"
#include "Board.h"

using namespace std; 

// Constructor for a new simulation
Simulation::Simulation(Configuration* config, int rank)
{
	numProcessors = config->GetSimConfig().numProcs;
	m_numDays = config->GetSimConfig().loops;
	m_numLoops = config->GetSimConfig().loopsPerDay;
    myBoard = new Board(rank, config->GetBoardConfig(), config->GetOrgConfig());
    myRank = rank;
}

void Simulation::RunMultipleSimulations(Configuration* config, int rank) {
	
	int numSims = config->GetSimConfig().numSimulations;
	for (int i = 0; i < numSims; i++) {
		if(!rank)cout << "Simulation " << i << endl;
 		auto sim = new Simulation(config, rank);
		sim->RunSimulation();
		delete sim;
	}
}

// Runs the full simulation for the specified number of "days"
void Simulation::RunSimulation()
{
	for (int i = 0; i < m_numDays; i++)
	{
		if (!myRank && i % 500 == 0) cout << i << endl;
		MPI_Barrier(MCW);
		// TODO: New day begins (maybe .newDay function)
		for (int j = 0; j < m_numLoops; j++) {
			trackBoardInfo(myBoard->liveForADay());
		}
	}
	GatherSimulationInfo();
	cout << "\n\n\n\n\n\n\n\n\n\n";

}

void Simulation::trackBoardInfo(Board::BoardInfo simInfo) {
	m_boardInfoList.push_back(simInfo);
}

// Package up relevant information to send to a single process, 
// so information can be aggregated and outputted
void Simulation::GatherSimulationInfo()
{
	if(!myRank) cout << "x\torgs\tsight\tspeed\tfood" << endl;
	for (int i = 0; i < m_numDays; i++) {
		double totalOrganisms = 0;
		double totalOrganismSpeed = 0;
		double avgOrganismSpeed;
		double totalOrganismSight = 0;
		double avgOrganismSight;
		double totalBoardFood = 0;
		double avgBoardFood;
		double* recvInfoArray = new double[numProcessors * 4];
		double* simInfoArray = m_boardInfoList.at(i).getSimInfoArray();
		MPI_Gather(simInfoArray, 4, MPI_DOUBLE, recvInfoArray, 4, MPI_DOUBLE, 0, MCW);
		if (!myRank) {
			for (int i = 0; i < numProcessors; i++) {
				int index = i * 4;
				totalOrganisms += recvInfoArray[index];
				totalOrganismSpeed += recvInfoArray[index + 1];
				totalOrganismSight += recvInfoArray[index + 2];
				totalBoardFood += recvInfoArray[index + 3];
			}
			avgOrganismSpeed = totalOrganismSpeed / totalOrganisms;
			avgOrganismSight = totalOrganismSight / totalOrganisms;
			avgBoardFood = totalBoardFood / numProcessors;
			//cout << "Day " << i << ": Total Organisms " << totalOrganisms << " Sight " << avgOrganismSight << " Speed " << avgOrganismSpeed << " Food " << avgOrganismFood << endl;
			cout << i << "\t" << totalOrganisms << "\t" << avgOrganismSight << "\t" << avgOrganismSpeed << "\t" << avgBoardFood << endl;
		}
		delete recvInfoArray, simInfoArray;
	}
}

// Output all relevant information into a .csv file for analysis
void Simulation::OutputSimulationResults()
{

}
