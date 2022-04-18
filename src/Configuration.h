#include <string>
#ifndef MAIN_CPP_CONFIGURATION_H
#define MAIN_CPP_CONFIGURATION_H

using namespace std;

struct SimConfig
{
    int loops; // Number of iterations to run the simulation
    int loopsPerDay; // Number of loops that constitute a 'day'
    int numProcs; // Number of processes (which will be the number of boards
    int numSimulations;
};

struct BoardConfig
{
    int length; // length / width of a board (always square)
    double foodDensity; // A measure of the amount of food on the board
    int numOrgs; // Number of organisms that start on a board
    int numBoards; // Number of boards - same as number of processors
    int foodSpawnedPerDay; // Amount of food that is spawned after each day
    bool spawnFoodPercentage; // Spawn food based on a percentage as opposed to a specific value
    double percentageFoodPerDay; // Food percentage to spawn each day
};

struct OrgConfig
{
    int speed; // Speed of an organism
    int sight; // Sight of an organism
    string distribution; // The distribution (we'll probably only consider normal and uniform)
};

class Configuration{
public:
    Configuration(OrgConfig org, BoardConfig board, SimConfig sim);

    OrgConfig GetOrgConfig();
    BoardConfig GetBoardConfig();
    SimConfig GetSimConfig();

private:
    OrgConfig orgConfig;
    BoardConfig boardConfig;
    SimConfig simConfig;

};


#endif //MAIN_CPP_CONFIGURATION_H
