#include "Organism.h"
#include "Board.h"

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
    m_locationList = std::vector<PairPointer>();
    m_locationList.push_back(GetCoordinatePair());
    m_requiresTransition = false;
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
    m_locationList = std::vector<PairPointer>();
    m_locationList.push_back(GetCoordinatePair());
    m_requiresTransition = false;
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

PairPointer Organism::GetCoordinatePair() 
{
    return PairPointer(new CoordPair(m_x_location, m_y_location));
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
    cout << "Organism ID: " << m_id << endl;
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

void Organism::setRequiresTransition() {
    m_requiresTransition = true;
}


bool Organism::requiresTransition() {
    return m_requiresTransition;
}

int *Organism::GetTransitionStatsArray()
{
    m_x_location = m_transitionX;
    m_y_location = m_transitionY;
    UpdateArray();
    return m_stats_array;
}

Organism::BoardMoves Organism::GetTransitionDirection(){
    return m_transitionDirection;
}

PairPointer Organism::GetTransitionLocation() {
    return PairPointer(new CoordPair(m_transitionX, m_transitionY));
}

void Organism::MoveToLocation(PairPointer location) {
    m_x_location = location->first;
    m_y_location = location->second;
    m_locationList.push_back(GetCoordinatePair());
    UpdateArray();
}

void Organism::setTransitionLocation(PairPointer location) {
    m_transitionX = location->first;
    m_transitionY= location->second;
}

/// <summary>
/// Move a random direction and determine if a transition is necessary
/// </summary>
/// <param name="boardSize">The size of the food board</param>
void Organism::moveRandomDirection(int boardSize) {
    auto time = static_cast<int>(chrono::high_resolution_clock::now()
        .time_since_epoch().count());
    srand(time);
    int random = rand() % 4;
    PairPointer newLocation;
    switch (random) {
    case 0: newLocation = PairPointer(new CoordPair(m_x_location + m_speed, m_y_location));
        break;
    case 1: newLocation = PairPointer(new CoordPair(m_x_location - m_speed, m_y_location));
        break;
    case 2: newLocation = PairPointer(new CoordPair(m_x_location, m_y_location + m_speed));
        break;
    case 3: newLocation = PairPointer(new CoordPair(m_x_location, m_y_location - m_speed));
        break;
    }
    if (newLocation->first >= boardSize) {
        m_transitionDirection = BoardMoves::RIGHT;
        newLocation->first = newLocation->first % boardSize;
        cout << "transition right\n\n" << endl;
    }
    else if (newLocation->first < 0) {
        m_transitionDirection = BoardMoves::LEFT;
        newLocation->first = newLocation->first % boardSize;
        if (newLocation->first < 0)newLocation->first += boardSize;
        cout << "transition left\n\n" << endl;
    }
    else if (newLocation->second >= boardSize) {
        m_transitionDirection = BoardMoves::DOWN;
        newLocation->second = newLocation->second % boardSize;
        cout << "transition Down\n\n" << endl;
    }
    else if (newLocation->second < 0) {
        m_transitionDirection = BoardMoves::UP;
        newLocation->second = newLocation->second % boardSize;
        if (newLocation->second < 0)newLocation->second += boardSize;
        cout << "transition UP\n\n" << endl;
    }
    else {
        m_transitionDirection = BoardMoves::NONE;
        MoveToLocation(newLocation);
        return;
    }
    setTransitionLocation(newLocation);
    setRequiresTransition();
}

// Determine distance between the organism and the destination
double calculateDistance(PairPointer organism, PairPointer destination) {
    double x = (double)organism->first - (double)destination->first;
    double y = (double)organism->second - (double)destination->second;
    return sqrt((x * x) + (y * y));
}

// Determine which direction to move the organism to get to the food
int calculateDesiredMovement(int organismLoc, int destination) {
    int adjustedMove = organismLoc - destination;
    return adjustedMove != 0 ? adjustedMove / abs(adjustedMove) : adjustedMove;
}


/// <summary>
/// Find the nearest food and move to it 
/// or move in a random direction and determine if a transition is necessary
/// </summary>
/// <param name="foodBoard">The board containing the food</param>
/// <param name="boardSize">The width/length of the food array</param>
/// <returns>Where the organism consumed food</returns>
PairPointer Organism::findAndConsumeFood(int** foodBoard, int boardSize) {
    int x = m_x_location;
    int y = m_y_location;
    int smallestX = (x - m_sight) > 0 ? x - m_sight : 0;
    int smallestY = (y - m_sight) > 0 ? y - m_sight : 0;
    int largestX = boardSize > (x + m_sight) ? x + m_sight : boardSize - 1;
    int largestY = boardSize > (y + m_sight) ? y + m_sight : boardSize - 1;

    PairPointer foodCoordinate = PairPointer(nullptr);
    double distance = 1000;
    // Check all coordinates within potentially visible distance
    for (int i = smallestY; i <= largestY; i++) {
        for (int j = smallestX; j <= largestX; j++) {
            auto potentialFood = PairPointer(new CoordPair(j, i));
            double potentialDistance = calculateDistance(GetCoordinatePair(), potentialFood);
            if (foodBoard[j][i] > 0 // Is there actually food there?
                && m_sight >= potentialDistance // Can we see it?
                && distance > potentialDistance) // Is it closer than the other food?
            {
                foodCoordinate = potentialFood;
                distance = potentialDistance;
            }
        }
    }
    // If no food was found within sight
    if (foodCoordinate == nullptr) {
        moveRandomDirection(boardSize);
        return nullptr;
    }
    // If the organism arrived at the nearest food
    if (moveClosestToFood(foodCoordinate)) {
        ConsumeFood(foodBoard[GetX()][GetY()]);
        return PairPointer(new CoordPair(GetX(), GetY()));
    }
    // No food was consumed
    return nullptr;
}

/// <summary>
/// Move as close to the food as possible
/// </summary>
/// <param name="pFoodLocation">The x,y coordinate location of the food</param>
/// <returns>Whether the organism arrived at the nearest food</returns>
bool Organism::moveClosestToFood(PairPointer pFoodLocation) {
    auto orgLocation = GetCoordinatePair();
    double distanceOfDestination = calculateDistance(orgLocation, pFoodLocation);
    int speed = GetSpeed();
    // Move to the food
    if (speed >= distanceOfDestination) {
        MoveToLocation(pFoodLocation);
        return true;
    }

    // Move as close to the food as possible
    auto pDestCoordinate = PairPointer(new CoordPair(pFoodLocation->first, pFoodLocation->second));
    while (distanceOfDestination > speed) {
        int adjustedX = calculateDesiredMovement(orgLocation->first, pDestCoordinate->first);
        pDestCoordinate->first = pDestCoordinate->first + adjustedX;
        distanceOfDestination = calculateDistance(orgLocation, pDestCoordinate);
        if (speed >= distanceOfDestination) break; 

        int adjustedY = calculateDesiredMovement(orgLocation->second, pDestCoordinate->second);
        pDestCoordinate->second = pDestCoordinate->second + adjustedY;
        distanceOfDestination = calculateDistance(orgLocation, pDestCoordinate);
    }
    MoveToLocation(pDestCoordinate);
    return false;
}

void Organism::printLocations() {
    cout << "Location List for organism " << m_id << endl;
    for (auto loc : m_locationList) {
        cout << loc->first << "," << loc->second << endl;
    }
}

void Organism::ConsumeFood(int food) {
    m_currentFood += food;
    m_stats_array[FOOD] = m_currentFood;
    cout << "Organism " << m_id << " consumed " << food << " food at " << m_x_location << "," << m_y_location << " And now has " << m_currentFood << " food" << endl;
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
