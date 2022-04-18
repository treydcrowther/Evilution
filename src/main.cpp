#include <iostream>
#include <mpi.h>
#include "Simulation.h"
#include "Configuration.h"

void OrganismDriverCode(int rank);

using namespace std;

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    if (size != 4 && size != 9 && size != 16 && size != 25) {
        cout << "Number of processes/boards must be either 4, 9, 16, 25" << endl;
        MPI_Finalize();
        return 1;
    }

    // SimConfig: loops, loopsPerDay, numProcs, numSimulationsToRun
    SimConfig sim = {5000, 1, size, 5};
    // BoardConfig: lengthOfBoard foodDensity, numInitialOrganisms, numBoards, foodSpawnedPerDay, spawnFoodByPercentage, percentageToSpawn
    BoardConfig board = {100, .5, 30, size, 8, true, .01};
    // OrgConfig: speed, sight, distribution(normal or uniform)
    OrgConfig org = {1, 1, "uniform"};

    auto config = new Configuration(org, board, sim);

    Simulation::RunMultipleSimulations(config,rank);

    MPI_Finalize();
    return 0;
}


