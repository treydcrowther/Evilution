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

// Runs the full simulation for the specified number of "days"
void Simulation::RunSimulation()
{
	for (int i = 0; i < m_numDays; i++)
	{
		MPI_Barrier(MCW);
		// TODO: New day begins (maybe .newDay function)
		for (int j = 0; j < m_numLoops; j++) {
			trackBoardInfo(myBoard->liveForADay());
		}
	}
	GatherSimulationInfo();
	
}

void Simulation::trackBoardInfo(Board::BoardInfo simInfo) {
	m_boardInfoList.push_back(simInfo);
}

// Package up relevant information to send to a single process, 
// so information can be aggregated and outputted
void Simulation::GatherSimulationInfo()
{
	if(!myRank) cout << "Day, Total Organisms, Avg Sight, Avg Speed, Avg Food" << endl;
	for (int i = 0; i < m_numDays; i++) {
		double totalOrganisms = 0;
		double totalOrganismSpeed = 0;
		double avgOrganismSpeed;
		double totalOrganismSight = 0;
		double avgOrganismSight;
		double totalOrganismFood = 0;
		double avgOrganismFood;
		double* recvInfoArray = new double[numProcessors * 4];
		double* simInfoArray = m_boardInfoList.at(i).getSimInfoArray();
		MPI_Gather(simInfoArray, 4, MPI_DOUBLE, recvInfoArray, 4, MPI_DOUBLE, 0, MCW);
		if (!myRank) {
			for (int i = 0; i < numProcessors; i++) {
				int index = i * 4;
				totalOrganisms += recvInfoArray[index];
				totalOrganismSpeed += recvInfoArray[index + 1];
				totalOrganismSight += recvInfoArray[index + 2];
				totalOrganismFood += recvInfoArray[index + 3];
			}
			avgOrganismSpeed = totalOrganismSpeed / totalOrganisms;
			avgOrganismSight = totalOrganismSight / totalOrganisms;
			avgOrganismFood = totalOrganismFood / totalOrganisms;
			//cout << "Day " << i << ": Total Organisms " << totalOrganisms << " Sight " << avgOrganismSight << " Speed " << avgOrganismSpeed << " Food " << avgOrganismFood << endl;
			cout << i << ", " << totalOrganisms << ", " << avgOrganismSight << ", " << avgOrganismSpeed << ", " << avgOrganismFood << endl;
		}
		delete recvInfoArray, simInfoArray;
	}
}

// Output all relevant information into a .csv file for analysis
void Simulation::OutputSimulationResults()
{

}
