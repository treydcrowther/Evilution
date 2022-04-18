#ifndef EVILUTION_BOARD_H
#define EVILUTION_BOARD_H

#include <string>
#include <set>
#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include "Organism.h"
#include "Configuration.h"

#define MCW MPI_COMM_WORLD
typedef std::shared_ptr<Organism> OrganismPointer;

class Board {
public:
    explicit Board(int rank, BoardConfig board, OrgConfig org);

    struct BoardInfo {
        double numOrganisms;
        double totalOrganismSpeed;
        double totalOrganismSight;
        double totalOrganismFood;
        double* getSimInfoArray() {
            return new double[4] {numOrganisms, totalOrganismSpeed, totalOrganismSight, totalOrganismFood};
        }
    };

    BoardInfo liveForADay();
    void printFoodArray();
    void printStats();
    int** getFoodBoard();
    int getBoardSize();

private:
    void spawnDailyFood();
    void spawnInitialFood(double foodDensity);
    void initializeFoodArray();
    void addOrganismToBoard(OrganismPointer pOrganism);
    void addFoodToBoard(int x, int y, int count);
    void sendAndReceiveOrganisms();
    void removeOrganismFromBoard(OrganismPointer pOrganism);
    void removeFood(PairPointer location);
    int determineRecipient(Organism::BoardMoves move);
    BoardInfo calculateDailySimulationInfo();
    
    
    int m_rank;
    int m_foodCount;
    int** m_foodArray;
    int m_foodSpawnedPerDay;
    int m_rowSize;
    int m_boardSize;
    int m_totalBoards;
    int m_nextOrganismId;
    bool m_spawnFoodByPercentage;
    double m_foodPercentagePerDay;

    std::vector<OrganismPointer> m_organisms;
    std::vector<OrganismPointer> m_organismsToSend;
};


#endif //EVILUTION_BOARD_H
