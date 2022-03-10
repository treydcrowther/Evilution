#ifndef EVILUTION_ORGANISM_H
#define EVILUTION_ORGANISM_H

#include <iostream>

class Organism {
public:
    explicit Organism(int* stats);
    
    explicit Organism(int id, int x, int y, int speed, int sight);

    static int GetArraySize();
    
    static int* RandomOrganismStats();

    void ConsumeFood(int food);

    void MoveToLocation(std::pair<int, int>* location);

    void UpdateArray();

    void PrintStats() const;

    //Getters
    int GetId();
    int GetX();
    int GetY();
    int GetSpeed();
    int GetSight();
    int GetCurrentFood();
    int* GetStatsArray();
    std::pair<int,int> GetCoordinatePair();

    //Setters
    void SetId(int id);
    void SetX(int x);
    void SetY(int y);
    void SetSpeed(int speed);
    void SetSight(int sight);

private:

    //The indices refer to each attributes location in the array
    enum StatsIndices
    {
        ID,
        X,
        Y,
        SPEED,
        SIGHT,
        FOOD
    };
    static int m_array_size;
    int m_id;
    int m_x_location;
    int m_y_location;
    int m_speed;
    int m_sight;
    int m_currentFood;

    int* m_stats_array;
    bool m_moreFood; // Potential Idea: If set to true the organism will go after more food as opposed to closer food
};


#endif //EVILUTION_ORGANISM_H
