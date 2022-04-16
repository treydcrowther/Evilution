#include "Board.h"
#include <iostream>
#include <mpi.h>
#include <cmath>
#include "RandomGenerator.h"

using namespace std;

/*Constructor for the board*/
Board::Board(int rank, BoardConfig board, OrgConfig org) {
	m_rank = rank;
	m_boardSize = board.length;
	m_totalBoards = board.numBoards;
	m_rowSize = sqrt(m_totalBoards);
	m_nextOrganismId = rank * 1000;
	m_foodSpawnedPerDay = board.foodSpawnedPerDay;
	initializeFoodArray();
	int initialOrganisms = board.numOrgs;

	for (int i = 0; i < initialOrganisms; i++) {
		addOrganismToBoard(OrganismPointer(
			new Organism(m_nextOrganismId++, Random::getRand(m_boardSize), Random::getRand(m_boardSize), org.speed, org.sight)));
	}
	spawnInitialFood(board.foodDensity);
}

int Board::getBoardSize() {
	return m_boardSize;
}

int** Board::getFoodBoard() {
	return m_foodArray;
}

Board::BoardInfo Board::calculateDailySimulationInfo() {
	double totalSight = 0;
	double totalSpeed = 0;
	double totalOrganisms = 0;
	double totalFood = 0;
	for (OrganismPointer op : m_organisms) {
		totalSight += op->GetSight();
		totalSpeed += op->GetSpeed();
		totalFood += op->GetCurrentFood();
	}
	totalOrganisms = m_organisms.size();
	auto simInfo = BoardInfo{ totalOrganisms, totalSpeed, totalSight, totalFood};
	//cout << "Rank " << m_rank << ": Total Organisms " << totalOrganisms << " Sight " << totalSight << " Speed " << totalSpeed << " Food " << totalFood << endl;
	return simInfo;
}

void Board::spawnDailyFood() {
	for (int i = 0; i < m_foodSpawnedPerDay; i++)
		m_foodArray[Random::getRand(m_boardSize)][Random::getRand(m_boardSize)] += 1;
}

void Board::spawnInitialFood(double foodDensity) {
	double numFood = (m_boardSize * m_boardSize) * foodDensity;
	for (int i = 0; i < numFood; i++)
		m_foodArray[Random::getRand(m_boardSize)][Random::getRand(m_boardSize)] += 1;
}

// Remove food from the board once it has been consumed
void Board::removeFood(PairPointer location)
{
	if (location == nullptr) return;
	int x = location->first;
	int y = location->second;
	m_foodCount -= m_foodArray[x][y];
	m_foodArray[x][y] = 0;
}

void Board::addFoodToBoard(int x, int y, int count) {
	m_foodArray[x][y] += count;
	m_foodCount += count;
}

void Board::addOrganismToBoard(OrganismPointer pOrganism) {
	m_organisms.push_back(pOrganism);
}

void Board::removeOrganismFromBoard(OrganismPointer pOrganism) {
	m_organisms.erase(std::remove(m_organisms.begin(), m_organisms.end(), pOrganism));
}


Board::BoardInfo Board::liveForADay() {
	std::vector<OrganismPointer> newOrganismChildren = std::vector<OrganismPointer>();
	std::vector<OrganismPointer> organismsToKill = std::vector<OrganismPointer>();
	for (auto pOrganism : m_organisms) {
		removeFood(pOrganism->findAndConsumeFood(getFoodBoard(), getBoardSize(), m_rank));
		// Kill off organisms who have not found food in 2 days
		if (!pOrganism->doesOrganismSurvive()) {
			organismsToKill.push_back(pOrganism);
			continue;
		}
		// Transition organisms to new boards if necessary
		if (pOrganism->requiresTransition()) m_organismsToSend.push_back(pOrganism);
		// Reproduce and add new organism to the board
		auto orgChild = pOrganism->reproduce(m_nextOrganismId);
		if (orgChild != nullptr) {
			m_nextOrganismId++;
			newOrganismChildren.push_back(OrganismPointer(new Organism(orgChild)));
		}
	}
	for (OrganismPointer op : newOrganismChildren) addOrganismToBoard(op);
	for (OrganismPointer op : organismsToKill) removeOrganismFromBoard(op);
	spawnDailyFood();
	sendAndReceiveOrganisms();
	return calculateDailySimulationInfo();
}

// Transition all organisms to new boards
void Board::sendAndReceiveOrganisms() {
	// Create list of organisms for each of the other processes
	auto listOfOrganisms = std::vector<std::vector<OrganismPointer>>();
	for (int i = 0; i < m_totalBoards; i++)
		listOfOrganisms.push_back(std::vector<OrganismPointer>());
	// Place organisms in correct containers to be sent to other boards
	for (auto pOrganism : m_organismsToSend) {
 		int recipient = determineRecipient(pOrganism->GetTransitionDirection());
		listOfOrganisms.at(recipient).push_back(pOrganism);
		removeOrganismFromBoard(pOrganism);
	}
	m_organismsToSend.clear();
	for (int i = 0; i < m_totalBoards; i++) {
		if (i == m_rank) continue;
		int numOrganismsToSend = listOfOrganisms.at(i).size();
		MPI_Send(&numOrganismsToSend, 1, MPI_INT, i, 0, MCW);
		int numOrganismsToRecv;
		MPI_Recv(&numOrganismsToRecv, 1, MPI_INT, i, 0, MCW, MPI_STATUS_IGNORE);
		// Send all the organisms
		for (auto organism : listOfOrganisms.at(i)) {
			int* organismStats = organism->GetTransitionStatsArray();
			MPI_Send(organismStats, Organism::GetArraySize(), MPI_INT, i, 0, MCW);
		}
		//Receive all the new organisms
		for (int j = 0; j < numOrganismsToRecv; j++) {
			int* organismStats = new int[Organism::GetArraySize()];
			MPI_Recv(organismStats, Organism::GetArraySize(), MPI_INT, i, 0, MCW, MPI_STATUS_IGNORE);
			//cout << "Organism " << organismStats[0] << " transitioned to board " << m_rank << " at location " << organismStats[1] << "," << organismStats[2] << endl;
			addOrganismToBoard(OrganismPointer(new Organism(organismStats)));
		}
	}
}

// Determine what board the organism should be sent to from the move type
int Board::determineRecipient(Organism::BoardMoves move) {
	int recipientRank = m_rank;
	switch (move) {
	case Organism::BoardMoves::UP: 
		recipientRank = m_rank - m_rowSize;
		if (recipientRank < 0) recipientRank += m_totalBoards;
		break;
	case Organism::BoardMoves::DOWN: 
		recipientRank = (m_rank + m_rowSize) % m_totalBoards;
		break;
	case Organism::BoardMoves::LEFT:
		recipientRank = m_rank - 1;
		if (recipientRank < 0) recipientRank += m_totalBoards;
		break;
	case Organism::BoardMoves::RIGHT:
		recipientRank = (m_rank + 1) % m_totalBoards;
		break;
	}
	return recipientRank;
}


void Board::initializeFoodArray() {
	m_foodArray = new int* [m_boardSize];
	for (int i = 0; i < m_boardSize; i++)
		m_foodArray[i] = new int[m_boardSize];

	for (int i = 0; i < m_boardSize; i++) {
		for (int j = 0; j < m_boardSize; j++) {
			m_foodArray[j][i] = 0;
		}
	}
}

/*Print the food array*/ 
void Board::printFoodArray() {
	cout << "  ";
	for (int i = 0; i < m_boardSize; i++)
		cout << i << " ";
	cout << endl;

	for (int i = 0; i < m_boardSize; i++) {
		cout << i << " ";
		for (int j= 0; j < m_boardSize; j++) {
			cout << m_foodArray[j][i] << " ";
		}
		cout << endl;
	}
}

/*Print the statistics of the current board*/ 
void Board::printStats() {
	cout << "Board " << m_rank << ":\nContains " << m_organisms.size() << " organisms \nContains " << m_foodCount << " food\n" << endl;
}
