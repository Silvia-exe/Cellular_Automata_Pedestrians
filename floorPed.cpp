
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
		std::cout << "Door added at: " << door[k][0] << "," << door[k][1] << "\n";
		std::cout << "Obstacle: " << obstacle[door[k][0]][door[k][1]] << "\n";
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

/*Not finished, will calculate and update the dynamic field*/
void floorPed::dynamicDecay() {
	//std::cout << "Entered dynamic decay" << "\n";
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			dynField[i][j] -= 1 * alpha;
		}
	}
}

/*Adds a pedestrian to the floor if the cell isnt occupied or has an obstacle*/
bool floorPed::addPed(pedestrian p1) {
	
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
	//std::cout << "entered isPedSafe" << "\n";
	for (int i = 0; i < door.size(); i++) {
		if (pedVec[p].position[0] == door[i][0] && pedVec[p].position[1] == door[i][1]) {
			pedVec[p].escape = 1;
		}
	}
	
}

/*Calculates the probability matrix of all pedestrians on the floor*/
void floorPed::calcProbMat(pedestrian& p1) {
	//std::cout << "entered calcProbMat" << "\n";
	int i = p1.position[0];
	int j = p1.position[1];
	double N = 0;
	p1.probMat[0][0] = p1.probMat[2][0] = p1.probMat[1][1] = p1.probMat[0][2] = p1.probMat[2][2] = 0;
	double a = expFunction(i, j - 1);
	double b = expFunction(i - 1, j);
	double c = expFunction(i, j + 1);
	double d = expFunction(i + 1, j);
	
	
	N = a + b + c + d;

	if (N == 0) {
		p1.probMat[1][0] = 0;
		p1.probMat[0][1] = 0;
		p1.probMat[1][2] = 0;
		p1.probMat[2][1] = 0;
	}
	else {
		p1.probMat[1][0] = (1/N) * a;
		p1.probMat[0][1] = (1/N) * b;
		p1.probMat[1][2] = (1/N) * c;
		p1.probMat[2][1] = (1/N) * d;
	}
}

/*Exponential function that is the basis on calculating the probability matrix*/
double floorPed::expFunction(int i, int j) {
	return exp(kD * dynField[i][j])*exp(kS*statField[i][j])*(1 - occupied[i][j])*obstacle[i][j];
}

/*
void floorPed::singleRun() {
	//std::cout << "Entered the single run" << "\n";
	dynamicDecay();
	//resetOccupied();
	int toErase = 0;
	for (int i = 0; i < pedVec.size(); i++) {
		calcProbMat(pedVec[i]);
		occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 0;
		pedVec[i].move();
		isPedSafe(i);

		if (pedVec[i].escape == 1){
			//occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 0;
			pedVec[i] = pedVec[pedVec.size() - toErase - 1];
			toErase++;
		}
		else {
			occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 1;
		}
	}
	clearPed(toErase);
}
*/

/*Really delicate function that 
1) calculates the probability matrix of the pedestrians
2) "empties" the floor
3) The pedestrians choose their next move and either take it or stay standing
4) Pedestrians are "added" back to the floor
5) The dynamic field is updated
NOTE: The pedestrians still arent saved, there is a problem with it*/
void floorPed::singleRun() {
	//std::cout << "Entered the single run" << "\n";
	dynamicDecay();
	for (int i = 0; i < pedVec.size(); i++) {
		calcProbMat(pedVec[i]);
		occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 0;
		pedVec[i].move();
		dynField[pedVec[i].position[0]][pedVec[i].position[1]] += 1;
		//isPedSafe(pedVec[i]);
		occupied[pedVec[i].position[0]][pedVec[i].position[1]] = 1;
	}
}

void floorPed::clearPed(int p) {
	for (int i = 0; i < p; i++)
	{
		pedVec.pop_back();
	}
	//std::cout << "Pedestrian " << p << "escaped" << "\n";
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
			/*for (int k = 0; k < door.size(); j++) {
				if (i == door[k][0] && j == door[k][1]) {
					file << 0;
				}
			}*/
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
	std::cout << ped << "\n";
	file.close();
}

int floorPed::numberOfPed() {
	return pedVec.size();
}
