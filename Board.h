#ifndef EVILUTION_BOARD_H
#define EVILUTION_BOARD_H

#include <string>
#include <set>
#include <vector>
#include <array>
#include <iostream>
#include "Organism.h"

#define MCW MPI_COMM_WORLD
constexpr auto ARRAYSIZE = 9;
typedef std::shared_ptr<Organism> OrganismPointer;

class Board {
public:
    explicit Board(int rank, int totalBoards, std::string configFile = "");

    void timePassing(int dayNumber);

    void printFoodArray();

    void printStats();

    int** getFoodBoard();

    int getBoardSize();

private:
    void initializeFoodArray();
    void addOrganismToBoard(OrganismPointer pOrganism);
    void addFoodToBoard(int x, int y, int count);
    void sendAndReceiveOrganisms();
    int determineRecipient(Organism::BoardMoves move);
    void removeOrganismFromBoard(OrganismPointer pOrganism);
    void removeFood(PairPointer location);

    int m_rank;
    int m_foodCount;
    int** m_foodArray;
    int m_rowSize;
    int m_boardSize = ARRAYSIZE;
    int m_totalBoards;

    std::vector<OrganismPointer> m_organisms;
    std::vector<OrganismPointer> m_organismsToSend;
};


#endif //EVILUTION_BOARD_H
