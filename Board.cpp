#include "Board.h"
#include <iostream>

using namespace std;

/// <summary>
/// Calculate the distance between two points
/// </summary>
/// <param name="organism">Where the organism is located</param>
/// <param name="destination">Where the destination is located</param>
/// <returns>The calculated distance</returns>
double calculateDistance(std::pair<int,int>* organism, std::pair<int, int>* destination) {
	double x = (double)organism->first - (double)destination->first;
	double y = (double)organism->second - (double)destination->second;
	return sqrt((x * x) + (y * y));
}

/// <summary>
/// Determine which direction to move destination to get it closer to organism
/// </summary>
/// <param name="organismLoc">Location of organism</param>
/// <param name="destination">Location of destination</param>
/// <returns>The direction the destination needs to move</returns>
int calculateDesiredMovement(int organismLoc, int destination) {
	int adjustedMove = organismLoc - destination;
	return adjustedMove != 0 ? adjustedMove / abs(adjustedMove) : adjustedMove;
}

/*Constructor for the board*/
Board::Board(int rank, string configFile) {
	this->m_rank = rank;
	int initialOrganisms = 1;
	m_foodCount = 1;
	if (configFile != "") {
		// Pull necessary information from config file

	}

	for (int i = 0; i < initialOrganisms; i++) {
		this->m_organisms.push_back(new Organism(0,5,5,2,8));
	}

	for (int i = 0; i < this->m_foodCount; i++) {
		this->m_foodArray[8][7] = 1;
		this->m_foodArray[7][0] = 1;
		this->m_foodArray[7][7] = 1;
		this->m_foodArray[0][3] = 1;
		this->m_foodArray[0][8] = 1;
		this->m_foodArray[5][4] = 1;
	}
}

/// <summary>
/// The driver for the entire board simulation
/// </summary>
/// <param name="dayNumber">What day number it is</param>
void Board::timePassing(int dayNumber) {
	for (auto pOrganism : m_organisms) {
		pOrganism->PrintStats();
		auto* foodLocation = findFoodInSight(pOrganism->GetCoordinatePair(), pOrganism->GetSight());
		if (foodLocation != nullptr && moveClosestToFood(pOrganism, foodLocation)) {
			pOrganism->ConsumeFood(m_foodArray[pOrganism->GetX()][pOrganism->GetY()]);
			m_foodArray[pOrganism->GetX()][pOrganism->GetY()] = 0;
		}
	}
}

/// <summary>
/// Finds food within sight of the organism
/// This searches a grid within potential sight distance then calculates the 
/// actual distance for every point to determine if it is close enough
/// </summary>
/// <param name="coordinates">The coordinates of the organism</param>
/// <param name="sight">The sight value of the organism</param>
/// <returns>The location on the board of the closest food or nullptr if no food is nearby</returns>
std::pair<int, int>* Board::findFoodInSight(std::pair<int,int> coordinates, int sight) {
	int x = coordinates.first;
	int y = coordinates.second;
	int smallestX = (x - sight) > 0 ? x - sight : 0;
	int smallestY = (y - sight) > 0 ? y - sight : 0;
	int largestX = m_arraySize > (x + sight) ? x + sight : m_arraySize - 1;
	int largestY = m_arraySize > (y + sight) ? y + sight : m_arraySize - 1;

	std::pair<int, int>* foodCoordinate = nullptr;
	double distance = 1000;
	// Check all coordinates within potentially visible distance
	for (int i = smallestY; i <= largestY; i++) {
		for (int j = smallestX; j <= largestX; j++) {
			auto potentialFood = new std::pair<int, int>(j, i);
			double potentialDistance = calculateDistance(&coordinates, potentialFood);
			if (m_foodArray[j][i] > 0 // Is there actually food there?
				&& sight >= potentialDistance // Can we see it?
				&& distance > potentialDistance) // Is it closer than the other food?
			{
				delete foodCoordinate;
				foodCoordinate = potentialFood;
				distance = potentialDistance;
			}
			else delete potentialFood;
		}
	}
	return foodCoordinate;
}

/// <summary>
/// Move the organism as close to the food as it can get
/// </summary>
/// <param name="pOrganism">Pointer to Organism object to determine sight and location</param>
/// <param name="pFoodLocation">Pointer to location of the food</param>
/// <returns>Whether the organism was able to move all the way to the food</returns>
bool Board::moveClosestToFood(Organism* pOrganism, std::pair<int, int>* pFoodLocation) {
	auto orgLocation = pOrganism->GetCoordinatePair();
	double distanceOfDestination = calculateDistance(&orgLocation, pFoodLocation);
	int speed = pOrganism->GetSpeed();
	// Move to the food
	if (speed >= distanceOfDestination) {
		pOrganism->MoveToLocation(pFoodLocation);
		delete pFoodLocation;
		return true;
	}

	// Move as close to the food as possible
	auto pDestCoordinate = new std::pair<int, int>(pFoodLocation->first, pFoodLocation->second);
	// Continue changing destination until it is within movable distance
	while (distanceOfDestination > speed){
		//Determine where to move in x-direction
		int adjustedX = calculateDesiredMovement(orgLocation.first, pDestCoordinate->first);
		// Move in the x-direction
		pDestCoordinate->first = pDestCoordinate->first + adjustedX; 
		distanceOfDestination = calculateDistance(&orgLocation, pDestCoordinate);
		if (speed >= distanceOfDestination) break; // Destination is within movable distance

		//Determine where to move in y-direction
		int adjustedY = calculateDesiredMovement(orgLocation.second, pDestCoordinate->second);
		// Move in the y-direction
		pDestCoordinate->second = pDestCoordinate->second + adjustedY; 
		distanceOfDestination = calculateDistance(&orgLocation, pDestCoordinate);
	}
	pOrganism->MoveToLocation(pDestCoordinate);
	delete pDestCoordinate, pFoodLocation;
	return false;
}

/// <summary>
/// Print the food array
/// </summary>
void Board::printFoodArray() {
	for (int i = 0; i < m_arraySize; i++) {
		for (int j= 0; j < m_arraySize; j++) {
			cout << m_foodArray[j][i] << " ";
		}
		cout << endl;
	}
}

/// <summary>
/// Print the statistics of the current board
/// </summary>
void Board::printStats() {
	cout << "Board " << m_rank << ":\nContains " << m_organisms.size() << " organisms \nContains " << m_foodCount << " food\n" << endl;
}
