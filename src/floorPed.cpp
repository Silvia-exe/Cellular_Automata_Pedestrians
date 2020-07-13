
#include "stdafx.h"
#include "floorPed.h"

/*Fills the matrixes with 0 in order to initialize them. 
Makes indexing easier*/
void floorPed::startMat() {
	std::vector<std::vector<double>> temp;
	for (int i = 0; i < x; i++) {
		std::vector<double> doubTemp;
		std::vector<bool> boolTemp;
		for (int j = 0; j < y; j++) {
			doubTemp.push_back(0);
			boolTemp.push_back(0);
		}
		dynField.push_back(doubTemp);
		statField.push_back(doubTemp);
		occupied.push_back(boolTemp);
		obstacle.push_back(boolTemp);
		temp.push_back(doubTemp);
	}

	for (int k = 0; k < door.size(); k++) {
		d_L.push_back(0);
		statFieldVect.push_back(temp);
	}
}

/*Calculates the needed fields, such as the static field
The first double for loops makes sure all cells are empty before adding walls, doors and pedestrians
Builds walls around the room and finally,
adds exits as doors*/
void floorPed::initMat() {
	statFieldInit();
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			obstacle[i][j] = 1;
		}
	}

	buildWall();

	for (int k = 0; k < door.size(); k++) {
		occupied[door[k][0]][door[k][1]] = 0;
		obstacle[door[k][0]][door[k][1]] = 1;
	}
}


/*Calculates the furthest away cell from every door*/
void floorPed::calcDL() {
	std::cout << "Entered calcDL" << "\n";
	std::vector<int> d(door.size());
	for (int i = 0; i < x; i++) {
		for (int j = 0 ; j < y; j++) {
			for(int k = 0; k < door.size(); k++){
				d[k] = sqrt(((door[k][0]-i)*(door[k][0] - i)) + ((door[k][1]-j)*(door[k][1]-j)));
				if (d[k] > d_L[k]) {
				d_L[k] = d[k];
				}
			}
		}
	}
}

/*Calculates the value of the static field for every door*/
void floorPed::calcStatF() {
	std::cout << "Entered calcStatF" << "\n";
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < door.size(); k++) {
				statFieldVect[k][i][j] = d_L[k] - sqrt((door[k][0] - i)*(door[k][0] - i) + (door[k][1] - j)*(door[k][1] - j));
			}
		}
	}	
}

/*Sets and calculates the final static field of the floor*/
void floorPed::statFieldInit() {
	std::cout << "Entered statFieldInit" << "\n";
	calcDL();
	calcStatF();
	if (door.size() == 1) {
		statField = statFieldVect[0];
	}
	else {
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				for (int k = 0; k < door.size(); k++) {
					if (statField[i][j] < statFieldVect[k][i][j]) {
						statField[i][j] = statFieldVect[k][i][j];
						
					}
				}
			}
		}
	}
}

/*Prints the static field DEBUG PURPOSES*/
void floorPed::printStatField() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << statField[i][j] << ":";
		}
		std::cout << "\n";
	}
}

/*Writes the static field of the floor to a text file*/
void floorPed::writeStatField2File(std::string fileName) {
	std::ofstream file;
	file.open(fileName + ".txt");

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == y - 1) {
				file << statField[i][j];
			}else{ 
				file << statField[i][j] << ":";
			}
			
		}
		file << "\n";
	}
	
	file.close();

}

/*Adds obstacles to the edges of the floor to create a wall and stop
the pedestrians to go out*/
void floorPed::buildWall() {
	for (int i = 0; i < x; i++) {
		obstacle[i][0] = 0;
		obstacle[i][y-1] = 0;
	}

	for (int j = 0; j < y; j++) {
		obstacle[0][j] = 0;
		obstacle[x-1][j] = 0;

	}
}

/*Not finished, will calculate and update the dynamic field. It will regulate only diffusion and decay.*/
/*void floorPed::dynamicDecay() {
	for (int i = 1; i < x-1; i++) {
		for (int j = 1; j < y-1; j++) {

			if (dynField[i][j] < 0) {
				dynField[i][j] = 0;
			}

			double temp = (rand()%10)*(0.1);

			if (temp <= beta) {
				dynField[i][j] -= 1;
				if (temp <= alpha + beta) {
					double difI = (rand() % 3) - 1;
					double difJ = (rand() % 3) -1;
					dynField[i][j] -= 1;
					dynField[difI][difJ] += 1;
				}
			}
			
			if (dynField[i][j] < 0) {
				dynField[i][j] = 0;
			}
		}
	}
}
*/

/*Adds a pedestrian to the floor if the cell isnt occupied or has an obstacle*/
bool floorPed::addPed(pedestrian & p1) {
	if (obstacle[p1.position[0]][p1.position[1]] == 0 || occupied[p1.position[0]][p1.position[1]] == 1) {
		return 0;
	}
	else {
		pedVec.push_back(p1);
		occupied[p1.position[0]][p1.position[1]] = 1;
		return 1;
	}
}

/*Checks if the pedestrian is standing at the door or not and "saves" it*/
void floorPed::isPedSafe(int p) {
	for (int i = 0; i < door.size(); i++) {
		if (pedVec[p].position[0] == door[i][0] && pedVec[p].position[1] == door[i][1]) {
			pedVec[p].escape = 1;
		}
	}
}

/*Calculates the probability matrix of all pedestrians on the floor*/
void floorPed::calcProbMat(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];
	double N = 0;

	pedVec[p].probMat[0][0] = pedVec[p].probMat[2][0] = pedVec[p].probMat[0][2] = pedVec[p].probMat[2][2] = 0;
	
	double l = expFunction(i, j - 1);
	double u = expFunction(i - 1, j);
	double r = expFunction(i, j + 1);
	double d = expFunction(i + 1, j);

	occupied[i][j] = 0; // This is done so that the probability can be calculated as if the cell wasnt occupied 
	double c = expFunction(i, j);
	occupied[i][j] = 1;

	N = l + u + r + d + c;
	double norm = 1 / N;

	pedVec[p].probMat[1][0] = (1/N) * l;
	pedVec[p].probMat[0][1] = (1/N) * u;
	pedVec[p].probMat[1][2] = (1/N) * r;
	pedVec[p].probMat[2][1] = (1/N) * d;
	pedVec[p].probMat[1][1] = (1/N) * c;
	
}

void floorPed::calcProbMatDiag(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];
	double N = 0;

	double l = expFunction(i, j - 1);
	double u = expFunction(i - 1, j);
	double r = expFunction(i, j + 1);
	double d = expFunction(i + 1, j);
	double ul = expFunction(i - 1, j - 1);
	double ur = expFunction(i - 1, j + 1);
	double bl = expFunction(i + 1, j - 1);
	double br = expFunction(i + 1, j + 1);

	occupied[i][j] = 0; // This is done so that the probability can be calculated as if the cell wasnt occupied 
	double c = expFunction(i, j);
	occupied[i][j] = 1;

	N = l + u + r + d + ur + ul + br + bl + c;
	double norm = 1 / N;

	pedVec[p].probMat[1][0] = norm * l;
	pedVec[p].probMat[0][1] = norm * u;
	pedVec[p].probMat[1][2] = norm * r;
	pedVec[p].probMat[2][1] = norm * d;
	pedVec[p].probMat[1][1] = norm * c;
	pedVec[p].probMat[0][0] = norm * ul;
	pedVec[p].probMat[0][2] = norm * ur;
	pedVec[p].probMat[2][0] = norm * bl;
	pedVec[p].probMat[2][2] = norm * br;

	/*for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			std::cout << pedVec[p].probMat[x][y] << ":";
		}
		std::cout << "\n";
	}
	std::cout << "\n";*/
}


/*Exponential function that is the basis on calculating the probability matrix*/
double floorPed::expFunction(int i, int j) {
	return exp(kD * dynField[i][j])*exp(kS*statField[i][j])*(1 - occupied[i][j])*obstacle[i][j];
}


/*Function that:
1) calculates the probability matrix of the pedestrians
2) "empties" the floor
3) The pedestrians choose their next move and either take it or stay standing
4) Pedestrians are "added" back to the floor
5) The dynamic field is updated
NOTE: The pedestrians still arent saved in this function*/
void floorPed::singleRun() {
	//dynamicDecay();
	for (int i = 0; i < pedVec.size(); i++) {
		calcProbMat(i);
		occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 0;
		pedVec[i].move();
		dynField[pedVec[i].position[0]][pedVec[i].position[1]] += 1;
		occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 1;
	}
}

void floorPed::singleRunSave() {
	//dynamicDecay();
	for (int p = 0; p < pedVec.size(); p++) {
		isPedSafe(p);
		if (pedVec[p].escape == 1) {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			//p++;
		}
		else {
			calcProbMat(p);
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			pedVec[p].move();
			dynField[pedVec[p].position[0]][pedVec[p].position[1]] += 1;
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 1;
		}
	}
}

void floorPed::singleRunAllTogether() {

	pedDecide();

	for (int p = 0; p < pedVec.size(); p++) {
		findNResolveConflicts(p);
		isPedSafe(p);
	}

	for (int p = 0; p < pedVec.size(); p++) {
		
		if (pedVec[p].escape == 1) {
		std::cout << "entered escape if" << "\n";
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
		}
		else {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			pedVec[p].position = pedVec[p].desiredMove;
			dynField[pedVec[p].position[0]][pedVec[p].position[1]] += 1;
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 1;
		}
	}
}

void floorPed::singleRunDiag() {

	pedDecideDiag();

	for (int p = 0; p < pedVec.size(); p++) {
		findNResolveConflicts(p);
	}

	for (int p = 0; p < pedVec.size(); p++) {
		occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
		pedVec[p].position = pedVec[p].desiredMove;
		//std::cout << pedVec[p].position[0] << ", " << pedVec[p].position[1] << "\n";
		occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 1;
		//std::cout << occupied[pedVec[p].position[0]][pedVec[p].position[1]] << "\n";
	}
	
	//std::cout << "Single Run finished" << "\n";

}

/*All pedestrians calculate their probability matrix, and from there they will choose which cell they will move to.*/
void floorPed::pedDecide() {
	for (int p = 0; p < pedVec.size(); p++) {
		calcProbMat(p);
		pedVec[p].chooseMove();
	}
}

void floorPed::pedDecideDiag() {
	for (int p = 0; p < pedVec.size(); p++) {
		calcProbMatDiag(p);
		pedVec[p].chooseMove();
	}
}

/*Converts the desired move of the "loser" in its actual position, so the "loser" wont move, 
and the "winner" will*/
void floorPed::findNResolveConflicts(int p) {
	for (int j = 0; j < pedVec.size(); j++) {
		if (j != p) {
			if (pedVec[p].desiredMove == pedVec[j].desiredMove) {
				if (pedVec[p].probMax > pedVec[j].probMax) {
					pedVec[j].desiredMove = pedVec[j].position;
				}
				else if (pedVec[p].probMax == pedVec[j].probMax) {
					if (rand() % 2 == 0) {
						pedVec[j].desiredMove = pedVec[j].position;
					}
					else {
						pedVec[p].desiredMove = pedVec[p].position;
					}
				}
				else {
					//pedVec[p].desiredMove = pedVec[p].position;
				}
			}
		}
	}	
}

void floorPed::clearPed(int p) {
	for (int i = 0; i < p; i++)
	{
		pedVec.pop_back();
	}
}

void floorPed::resetOccupied() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			occupied[i][j] = 0;
		}
	}
}

void floorPed::printMovements() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (obstacle[i][j] == 0) {
				std::cout << -1 << ":";
			}
			else {
				std::cout << occupied[i][j] << ":";
			}
		}
		std::cout << "\n";
	}
}

void floorPed::writeMovements2File(std::string fileName) {
	std::ofstream file;
	int ped = 0;
	file.open(fileName + ".txt");
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == y-1) {
				if (obstacle[i][j] == 0) {
					file << -1;
				}
				else {
					file << occupied[i][j];
				}
			}
			else {
				if (obstacle[i][j] == 0) {
					file << -1 << ":";
				}
				else {
					file << occupied[i][j] << ":";
					if (occupied[i][j] == 1) {
						ped++;
					}
				}
			}
		}
		file << "\n";
	}
	file.close();
	std::cout <<"ped: " <<  ped << "\n";
}

int floorPed::numberOfPed() {
	return pedVec.size();
}

int floorPed::numberOfSavedPed() {
	int savedPed = 0;
	for (int i = 0; i < pedVec.size(); i++) {
		if (pedVec[i].escape == 1) {
			savedPed++;
		}
	}
	return savedPed;
}
