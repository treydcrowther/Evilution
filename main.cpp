#include <iostream>
#include <mpi.h>
#include "Organism.h"
#define MCW MPI_COMM_WORLD

void OrganismDriverCode(int rank);

/*
 * Design issue:
 * I want this number to be a static member variables of the Organism class,
 * but I kept getting weird compile errors. This works for now.
 */
int numAttributes = 4;

using namespace std;

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MCW, &rank);
    MPI_Comm_size(MCW, &size);

    //OrganismDriverCode(rank);

    MPI_Finalize();
    return 0;
}

void OrganismDriverCode(int rank)
{
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

