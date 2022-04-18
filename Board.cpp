#include "Board.h"
#include <iostream>
#include <mpi.h>

using namespace std;

/*Constructor for the board*/
Board::Board(int rank, int totalBoards, string configFile) {
	m_rank = rank;
	m_totalBoards = totalBoards;
	m_rowSize = sqrt(m_totalBoards);
	initializeFoodArray();

	int initialOrganisms = 1;
	if (configFile != "") {
		// Pull necessary information from config file

	}
	if(rank == 3)
		addOrganismToBoard(OrganismPointer(new Organism(0, 5, 5, 2, 8)));
	addFoodToBoard(8, 7, 2);
	addFoodToBoard(7, 0, 1);
	addFoodToBoard(7, 7, 1);
	addFoodToBoard(0, 3, 1);
	addFoodToBoard(0, 8, 1);
	addFoodToBoard(5, 4, 1);
}


int Board::getBoardSize() {
	return m_boardSize;
}

int** Board::getFoodBoard() {
	return m_foodArray;
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


void Board::timePassing(int dayNumber) {
	for (auto pOrganism : m_organisms) {
		cout << "Day " << dayNumber << " for board " << m_rank << endl;
		removeFood(pOrganism->findAndConsumeFood(getFoodBoard(), getBoardSize()));
		if (pOrganism->requiresTransition()) m_organismsToSend.push_back(pOrganism);
		pOrganism->PrintStats();
	}
	MPI_Barrier(MCW);
	sendAndReceiveOrganisms();
}

// Transition all organisms to new boards
void Board::sendAndReceiveOrganisms() {
	auto listOfOrganisms = std::vector<std::vector<OrganismPointer>>();
	for (int i = 0; i < m_totalBoards; i++)
		listOfOrganisms.push_back(std::vector<OrganismPointer>());

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
			cout << "Organism " << organismStats[0] << " transitioned to board " << m_rank << " at location " << organismStats[1] << "," << organismStats[2] << endl;
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
	m_foodArray = new int* [10];
	for (int i = 0; i < m_boardSize; i++)
		m_foodArray[i] = new int[10];

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
