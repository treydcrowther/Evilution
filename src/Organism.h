#ifndef EVILUTION_ORGANISM_H
#define EVILUTION_ORGANISM_H

#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

typedef std::pair<int, int> CoordPair;
typedef std::shared_ptr<CoordPair> PairPointer;

#define FOODTOSURVIVE -2
#define FOODEATENPERDAY 1

class Organism {
public:
    //These are used to send an organism to a different board
    enum class BoardMoves
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    explicit Organism(int* stats);
    explicit Organism(int id, int x, int y, int speed, int sight);
    static int GetArraySize();
    static int* RandomOrganismStats(int id, int speed = -1, int sight = -1);
    PairPointer findAndConsumeFood(int** foodBoard, int boardSize, int rank);
    bool doesOrganismSurvive();
    int* reproduce(int nextId);
    void printLocations();
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
    int* GetTransitionStatsArray();
    PairPointer GetCoordinatePair();
    PairPointer GetTransitionLocation();
    BoardMoves GetTransitionDirection();
    bool requiresTransition();

    //Setters
    void SetId(int id);
    void SetX(int x);
    void SetY(int y);
    void SetSpeed(int speed);
    void SetSight(int sight);

private:
    int mutate();
    bool moveClosestToFood(PairPointer pFoodLocation, int rank);
    void ConsumeFood(int food);
    void MoveToLocation(PairPointer location);
    void moveRandomDirection(int boardSize);
    void setTransitionLocation(PairPointer location);
    void setRequiresTransition();
    void removeDailyFoodFromOrganism();

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
    int m_foodPerDay = FOODEATENPERDAY;
    int m_requiredFoodToSurvive = FOODTOSURVIVE;

    BoardMoves m_transitionDirection = BoardMoves::NONE;
    int m_transitionX;
    int m_transitionY;
    bool m_requiresTransition;

    int* m_stats_array;
    bool m_moreFood; // Potential Idea: If set to true the organism will go after more food as opposed to closer food
    std::vector<PairPointer> m_locationList;
};

#endif //EVILUTION_ORGANISM_H
