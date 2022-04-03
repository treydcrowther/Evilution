#include <string>
#ifndef MAIN_CPP_CONFIGURATION_H
#define MAIN_CPP_CONFIGURATION_H

using namespace std;

struct SimConfig
{
    int loops; // Number of iterations to run the simulation
    int loopsPerDay; // Number of loops that constitute a 'day'
    int numProcs; // Number of processes (which will be the number of boards
};

struct BoardConfig
{
    int size; // length / width of a board (always square)
    double foodDensity; // A measure of the amount of food on the board
    int numOrgs; // number of organisms that start on a board
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

private:
    OrgConfig orgConfig;
    BoardConfig boardConfig;
    SimConfig simConfig;

};


#endif //MAIN_CPP_CONFIGURATION_H
