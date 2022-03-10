#include "Organism.h"
#include <iostream>

using namespace std;

int Organism::m_array_size = 4;

Organism::Organism(int *stats)
{
    m_x_location = stats[m_X_INDEX];
    m_y_location = stats[m_Y_INDEX];
    m_speed = stats[m_SPEED_INDEX];
    m_sight = stats[m_SIGHT_INDEX];
    m_stats_array = new int[m_array_size];
    UpdateArray();
}

void Organism::UpdateArray()
{
    m_stats_array[m_X_INDEX] = m_x_location;
    m_stats_array[m_Y_INDEX] = m_y_location;
    m_stats_array[m_SPEED_INDEX] = m_speed;
    m_stats_array[m_SIGHT_INDEX] = m_sight;
}

int Organism::GetArraySize()
{
    return m_array_size;
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

void Organism::SetX(int x)
{
    m_x_location = x;
    m_stats_array[m_X_INDEX] = m_x_location;
}

void Organism::SetY(int y)
{
    m_y_location = y;
    m_stats_array[m_Y_INDEX] = m_y_location;
}

void Organism::SetSpeed(int speed)
{
    m_speed = speed;
    m_stats_array[m_SPEED_INDEX] = m_speed;
}

void Organism::SetSight(int sight)
{
    m_sight = sight;
    m_stats_array[m_SIGHT_INDEX] = m_sight;
}

void Organism::PrintStats() const
{
    cout << "X Location: " << m_x_location << endl;
    cout << "Y Location: " << m_y_location << endl;
    cout << "Speed: " << m_speed << endl;
    cout << "Sight: " << m_sight << endl;
}

int *Organism::GetStatsArray()
{
    return m_stats_array;
}

