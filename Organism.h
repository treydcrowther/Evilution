#ifndef EVILUTION_ORGANISM_H
#define EVILUTION_ORGANISM_H

#include <iostream>

class Organism {
public:
    explicit Organism(int* stats);
    
    explicit Organism(int x, int y, int speed, int sight);

    void UpdateArray();

    static int GetArraySize();

    static int* RandomOrganismStats();

    void MoveToLocation(std::pair<int, int>* location);

    void ConsumeFood(int food);

    void PrintStats() const;

    //Getters
    int GetX();
    int GetY();
    int GetSpeed();
    int GetSight();
    int* GetStatsArray();
    bool WantsMoreFood();
    std::pair<int,int> GetCoordinatePair();

    //Setters
    void SetX(int x);
    void SetY(int y);
    void SetSpeed(int speed);
    void SetSight(int sight);

private:
    static int m_array_size;
    //The indices refer to each attributes location in the array
    int m_id;
    int m_x_location;
    const int m_X_INDEX = 0;
    int m_y_location;
    const int m_Y_INDEX = 1;
    int m_speed;
    const int m_SPEED_INDEX = 2;
    int m_sight;
    const int m_SIGHT_INDEX = 3;
    int* m_stats_array;
    int m_currentFood;
    bool m_moreFood; // Potential Idea: If set to true the organism will go after more food as opposed to closer food
};


#endif //EVILUTION_ORGANISM_H
