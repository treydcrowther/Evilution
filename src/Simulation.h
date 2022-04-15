#ifndef SIMULATION_CPP_H
#define SIMULATION_CPP_H
#include <string>
#include "Configuration.h"
#include "Board.h"


class Simulation {
public:
	explicit Simulation(Configuration* config, int rank);

	void RunSimulation(int numDays);
	void GatherSimulationInfo();
	void OutputSimulationResults();

private:
	Board* myBoard;
	int myRank;
	int numProcessors;
	std::string configPath;
};

#endif //SIMULATION_CPP_H
