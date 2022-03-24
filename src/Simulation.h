#pragma once
#include <string>


class Simulation {
public:
	Simulation(int rank, int size, std::string configFilePath = "");
	void RunSimulation(int numDays);
	void GatherSimulationInfo();
	void OutputSimulationResults();

private:
	Board* myBoard;
	int myRank;
	int numProcessors;
	std::string configPath;
};
