#ifndef SIMULATION_CPP_H
#define SIMULATION_CPP_H
#include <string>
#include "Configuration.h"
#include "Board.h"


class Simulation {
public:
	explicit Simulation(Configuration* config, int rank);

	void RunSimulation();
	void GatherSimulationInfo();
	void OutputSimulationResults();

private:
	Board* myBoard;
	int myRank;
	int numProcessors;
	int m_numDays;
	int m_numLoops;
	int m_numSimulations;
	std::string configPath;
	std::vector<Board::BoardInfo> m_boardInfoList;
	
	void trackBoardInfo(Board::BoardInfo simInfo);
};

#endif //SIMULATION_CPP_H
