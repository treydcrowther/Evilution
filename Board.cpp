#include "Board.h"
#include <iostream>

using namespace std;

double calculateDistance(std::pair<int,int>* organism, std::pair<int, int>* food) {
	double x = (double)organism->first - (double)food->first;
	double y = (double)organism->second - (double)food->second;
	return sqrt((x * x) + (y * y));
}

double calculateMaximumX(double maxSight, int yPosDistance) {
	double distance = (maxSight * maxSight) - ((double)yPosDistance * (double)yPosDistance);
	if (distance < 0) return -1;
	return sqrt(distance);
}


Board::Board(int rank, string configFile) {
	this->m_rank = rank;
	int initialOrganisms = 1;
	m_foodCount = 1;
	if (configFile != "") {
		// Pull necessary information from config file

	}

	for (int i = 0; i < initialOrganisms; i++) {
		this->m_organisms.push_back(new Organism(5,5,5,8));
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

void Board::TimePassing(int dayNumber) {
	for (auto* pOrganism : m_organisms) {
		pOrganism->PrintStats();
		auto* foodLocation = FindFoodInSight(pOrganism->GetCoordinatePair(), pOrganism->GetSight());
		if (foodLocation != nullptr && MoveClosestToFood(pOrganism, foodLocation)) {
			pOrganism->ConsumeFood(m_foodArray[pOrganism->GetX()][pOrganism->GetY()]);
			m_foodArray[pOrganism->GetX()][pOrganism->GetY()] = 0;
		}
	}
}

std::pair<int, int>* Board::FindFoodInSight(std::pair<int,int> coordinates, int sight) {
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
		// Calculate the furthest distance it can see in the X direction given the current Y-Coordinate
		double maxX = calculateMaximumX(sight, y - i);
		for (int j = smallestX; j <= largestX; j++) {
			auto potentialFood = new std::pair<int, int>(j, i);
			if (m_foodArray[j][i] > 0 // Make sure there is actually food there
				&& maxX >= abs(x - j) // Ensure this coordinate is within visible distance
				&& distance > calculateDistance(&coordinates, potentialFood)) // Make sure it's closer than the other food
			{
				delete foodCoordinate;
				foodCoordinate = potentialFood;
				distance = calculateDistance(&coordinates, potentialFood);
			}
			else delete potentialFood;
		}
	}
	return foodCoordinate;
}


bool Board::MoveClosestToFood(Organism* pOrganism, std::pair<int, int>* pFoodLocation) {
	double distanceOfDestination = calculateDistance(&pOrganism->GetCoordinatePair(), pFoodLocation);
	int speed = pOrganism->GetSpeed();
	if (speed >= distanceOfDestination) {
		pOrganism->MoveToLocation(pFoodLocation);
		delete pFoodLocation;
		return true;
	}
	// Move as close to the food as possible
	auto pDestCoordinate = new std::pair<int, int>(pFoodLocation->first, pFoodLocation->second);
	// Continue changing destination until it is within movable distance
	while (distanceOfDestination > speed){
		//Determine which direction to move along x-axis
		int adjustedX = pOrganism->GetCoordinatePair().first - pDestCoordinate->first;
		if (adjustedX != 0) adjustedX = adjustedX / abs(adjustedX); 
		// Move along the x-axis
		pDestCoordinate->first = pDestCoordinate->first + adjustedX; 
		distanceOfDestination = calculateDistance(&pOrganism->GetCoordinatePair(), pDestCoordinate);
		if (speed >= distanceOfDestination) break; // Destination is within movable distance

		//Determine which direction to move along y-axis
		int adjustedY = pOrganism->GetCoordinatePair().second - pDestCoordinate->second;
		if (adjustedY != 0) adjustedY = adjustedY / abs(adjustedY); 
		// Move along the y-axis
		pDestCoordinate->second = pDestCoordinate->second + adjustedY; 
		distanceOfDestination = calculateDistance(&pOrganism->GetCoordinatePair(), pDestCoordinate);
	}
	pOrganism->MoveToLocation(pDestCoordinate);
	// Dang memory management
	delete pDestCoordinate, pFoodLocation;
	return false;
}

void Board::PrintFoodArray() {
	for (int i = 0; i < m_arraySize; i++) {
		for (int j= 0; j < m_arraySize; j++) {
			cout << m_foodArray[j][i] << " ";
		}
		cout << endl;
	}
}

void Board::PrintStats() {
	cout << "Board " << m_rank << ":\nContains " << m_organisms.size() << " organisms \nContains " << m_foodCount << " food\n" << endl;
}
