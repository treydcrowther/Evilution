#ifndef EVILUTION_BOARD_H
#define EVILUTION_BOARD_H

#include <string>
#include <set>
#include "Organism.h"
#include <vector>
#include <array>
#include <iostream>

constexpr auto ARRAYSIZE = 9;

class Board {
public:
    explicit Board(int rank, std::string configFile = "");

    void timePassing(int dayNumber);

    std::pair<int, int>* findFoodInSight(std::pair<int, int> coordinates, int sight);

    bool moveClosestToFood(Organism* organism, std::pair<int,int>* pFoodLocation);

    void printFoodArray();

    void printStats();

private:
    int m_rank;
    int m_arraySize = ARRAYSIZE;
    int m_foodArray[ARRAYSIZE][ARRAYSIZE] = { 0 };
    int m_foodCount;
    std::vector<Organism*> m_organisms;
    std::vector<Organism*> m_organismsToSend;
};


#endif //EVILUTION_BOARD_H
