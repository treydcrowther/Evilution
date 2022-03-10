#include "Organism.h"

using namespace std;

int Organism::m_array_size = 6;

Organism::Organism(int *stats)
{
    m_id = stats[ID];
    m_x_location = stats[X];
    m_y_location = stats[Y];
    m_speed = stats[SPEED];
    m_sight = stats[SIGHT];
    m_currentFood = stats[FOOD];
    m_stats_array = new int[m_array_size];
    UpdateArray();
}

Organism::Organism(int id, int x, int y, int speed, int sight)
{
    m_id = id;
    m_x_location = x;
    m_y_location = y;
    m_speed = speed;
    m_sight = sight;
    m_currentFood = 0;
    m_stats_array = new int[m_array_size];
    UpdateArray();
}

void Organism::UpdateArray()
{
    m_stats_array[ID] = m_id;
    m_stats_array[X] = m_x_location;
    m_stats_array[Y] = m_y_location;
    m_stats_array[SPEED] = m_speed;
    m_stats_array[SIGHT] = m_sight;
    m_stats_array[FOOD] = m_currentFood;
}

int Organism::GetArraySize()
{
    return m_array_size;
}

int Organism::GetId()
{
    return m_id;
}

int Organism::GetX()
{
    return m_x_location;
}

int Organism::GetY()
{
    return m_y_location;
}

int Organism::GetSpeed()
{
    return m_speed;
}

int Organism::GetSight()
{
    return m_sight;
}

int Organism::GetCurrentFood() 
{
    return m_currentFood;
}

std::pair<int, int> Organism::GetCoordinatePair() 
{
    return std::pair<int, int>(m_x_location, m_y_location);
}

void Organism::SetId(int id) {
    m_id = id;
    m_stats_array[ID] = m_id;
}

void Organism::SetX(int x)
{
    m_x_location = x;
    m_stats_array[X] = m_x_location;
}

void Organism::SetY(int y)
{
    m_y_location = y;
    m_stats_array[Y] = m_y_location;
}

void Organism::SetSpeed(int speed)
{
    m_speed = speed;
    m_stats_array[SPEED] = m_speed;
}

void Organism::SetSight(int sight)
{
    m_sight = sight;
    m_stats_array[SIGHT] = m_sight;
}

void Organism::PrintStats() const
{
    cout << "ID: " << m_id << endl;
    cout << "X Location: " << m_x_location << endl;
    cout << "Y Location: " << m_y_location << endl;
    cout << "Speed: " << m_speed << endl;
    cout << "Sight: " << m_sight << endl;
    cout << "Current Food: " << m_currentFood << endl;
}

int *Organism::GetStatsArray()
{
    return m_stats_array;
}

void Organism::MoveToLocation(std::pair<int, int>* location) {
    m_x_location = location->first;
    m_y_location = location->second;
    UpdateArray();
    cout << "new location is " << m_x_location << "," << m_y_location << endl;
}

void Organism::ConsumeFood(int food) {
    m_currentFood += food;
    m_stats_array[FOOD] = m_currentFood;
}

int* Organism::RandomOrganismStats() {
    int numAttributes = Organism::GetArraySize();
    int* stats = new int[numAttributes];
    for (int i = 0; i < numAttributes; i++)
    {
        stats[i] = i * 2 + 1;
    }
    return stats;
}
