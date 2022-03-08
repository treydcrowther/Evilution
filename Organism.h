#ifndef EVILUTION_ORGANISM_H
#define EVILUTION_ORGANISM_H


class Organism {
public:
    explicit Organism(int* stats);

    void UpdateArray();

    int GetArraySize();

    void PrintStats() const;

    //Getters
    int GetX();
    int GetY();
    int GetSpeed();
    int GetSight();
    int* GetStatsArray();

    //Setters
    void SetX(int x);
    void SetY(int y);
    void SetSpeed(int speed);
    void SetSight(int sight);

private:
    int m_array_size;
    //The indices refer to each attributes location in the array
    int m_x_location;
    const int m_X_INDEX = 0;
    int m_y_location;
    const int m_Y_INDEX = 1;
    int m_speed;
    const int m_SPEED_INDEX = 2;
    int m_sight;
    const int m_SIGHT_INDEX = 3;
    int* m_stats_array;
};


#endif //EVILUTION_ORGANISM_H
