#include <iostream>
#include <mpi.h>
#include "Board.h"
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

    if (size != 4 && size != 9 && size != 16) {
        cout << "Size must be either 4, 9, 16, 25, 36..." << endl;
        return 1;
    }

    SimConfig sim = {100, 10, size};
    BoardConfig board = {3, 1.0, 10, size};
    OrgConfig org = {1, 1, "uniform"};

    auto config = new Configuration(org, board, sim);
    auto simulation = new Simulation(config, rank);

    //OrganismDriverCode(rank);    

    MPI_Finalize();
    return 0;
}

void OrganismDriverCode(int rank)
{
    int numAttributes = Organism::GetArraySize();
    int* stats = new int[numAttributes];
    if(rank == 0)
    {
        for(int i = 0; i < numAttributes; i++)
        {
            stats[i] = i*2-1;
        }

        auto newOrganism = new Organism(stats);
        newOrganism->SetSpeed(12);
        MPI_Send(newOrganism->GetStatsArray(), numAttributes, MPI_INT, 1, 0, MCW);
    }
    else
    {
        MPI_Recv(stats, numAttributes, MPI_INT, 0, MPI_ANY_TAG, MCW, MPI_STATUS_IGNORE);
        auto newOrganism = new Organism(stats);
        newOrganism->PrintStats();
    }

    delete[] stats;
}

