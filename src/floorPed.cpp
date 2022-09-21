#define DOOR_IN_WALL
#define EPSILON 0.00005

#include "stdafx.h"
#include "floorPed.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*____________________UTILITY FUNCTIONS____________________
Basic tools used by the class*/

/*Gets a random integer between min and max-1*/
int getRandomInt(int min, int max) {
	return (rand() % (max - min)) + min;
}


/*Gets a random floating number between 0 and 1*/
double getRandom01() {
	return ceil(((double)rand() / RAND_MAX) * 10) / 10;
}


/*Gets a random number between 0 and 1 with a selected amount of decimal spaces*/
double randomNumber_01(int decimalSpaces) {
	return floor((getRandom01() * pow(10, decimalSpaces))) / pow(10, decimalSpaces);
}

/*Converts an x,y coordinate unto a single index.*/
int floorPed::coord2Indx(int p) {
	int i = pedVec[p].desiredMove[0];
	int j = pedVec[p].desiredMove[1];
	return j + i * (x - 1);
}

/*Deletes the selected pedestrian p from the vector.*/
void floorPed::clearPed(int p) {
	for (int i = 0; i < p; i++)
	{
		pedVec.pop_back();
	}
}

/*Clears the whole pedestrian vector.*/
void floorPed::erasePed() {
	pedVec.clear();
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*____________________INITIALIZING FUNCTIONS____________________
Build the floor and the matrixes. Declares and initializes (calculates) initial matrix values*/

/*Fills the matrixes with 0 in order to initialize them.
Makes indexing easier.*/
void floorPed::startMat() {
	std::vector<std::vector<double>> temp;
	for (int i = 0; i < x; i++) {
		std::vector<double> doubTemp;
		std::vector<bool> boolTemp;
		std::vector<int>intTemp;
		for (int j = 0; j < y; j++) {
			doubTemp.push_back(0);
			boolTemp.push_back(0);
			intTemp.push_back(0);
		}
		dynField.push_back(doubTemp); //Initializes dynamic Field
		auxDynField.push_back(doubTemp); //Initializes auxiliary dynamic field
		statField.push_back(doubTemp); //Initializes static Field
		occupied.push_back(boolTemp); //Initializes occupied Field
		obstacle.push_back(boolTemp); //Initizalizes obstacle Field
		groupMatrix.push_back(intTemp); //Initializes a matrix that shows the group each pedestrian is member of
		temp.push_back(doubTemp);
	}

	for (int k = 0; k < door.size(); k++) {
		d_L.push_back(0);
		statFieldVect.push_back(temp);
	}
}

/*Calculates the needed fields, such as the static field
The first for-loops makes sure all cells have no obstacles (obstacle = 1) before adding walls, doors and pedestrians
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

/*Fills the group matrix with the group of each pedestrian, makes following the behaviour of each group easier
Not yet in use, ignore for now*/
void floorPed::groupMatrixFill() {
	for (int p = 0; p < pedVec.size(); p++) {
		groupMatrix[pedVec[p].position[0]][pedVec[p].position[1]] = pedVec[p].groupNumber;
	}
}

/*Initializes the conflict vector. It is a vector of vectors which holds the pedestrian's desired move. It will have x*y pedestrian vectors*/
void floorPed::initConflictVec() {
	std::vector<pedestrian*> temp;
	for (int i = 0; i < x * y; i++) {
		conflictVec.push_back(temp);
	}
}

/*Calculates the furthest away cell from every door*/
void floorPed::calcDL() {
	std::vector<int> d(door.size());
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < door.size(); k++) {
				d[k] = sqrt(((door[k][0] - i) * (door[k][0] - i)) + ((door[k][1] - j) * (door[k][1] - j)));
				if (d[k] > d_L[k]) {
					d_L[k] = d[k];
				}
			}
		}
	}
}

/*Calculates the value of the static field for every door*/
void floorPed::calcStatF() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < door.size(); k++) {
				statFieldVect[k][i][j] = d_L[k] - sqrt((door[k][0] - i) * (door[k][0] - i) + (door[k][1] - j) * (door[k][1] - j));
			}
		}
	}
}

/*Sets and calculates the final static field of the floor*/
void floorPed::statFieldInit() {
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

/*Adds obstacles to the edges of the floor to create a wall and stop
the pedestrians to go out*/
void floorPed::buildWall() {
	for (int i = 0; i < x; i++) {
		obstacle[i][0] = 0;
		obstacle[i][y - 1] = 0;
	}

	for (int j = 0; j < y; j++) {
		obstacle[0][j] = 0;
		obstacle[x - 1][j] = 0;
	}
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*____________________ALGORITHM FUNCTIONS____________________
Functions directly used to run any of the single runs programed*/

/*Adds a pedestrian to the floor if the cell isn't occupied or has an obstacle*/
bool floorPed::addPed(pedestrian& p1) {
	if (obstacle[p1.position[0]][p1.position[1]] == 0 || occupied[p1.position[0]][p1.position[1]] == 1) {
		return 0;
	}
	else {
		pedVec.push_back(p1);
		occupied[p1.position[0]][p1.position[1]] = 1;
		return 1;
	}
}

/*Fills up the floor with n number of randomly placed pedestrians*/
void floorPed::ranPed(int n) {

	if (n >= (x - 2) * (y - 2)) {
		std::cout << "Thats too many" << "\n";
	}
	for (int i = 0; i < n; i++) {

		pedestrian temp = pedestrian(1 + rand() % (x - 2), 1 + rand() % (y - 2));
		if (addPed(temp) == 0) {
			i--;
		}
	}
}

/*Fills the room to a certain percentage. 0.3 will fill the room to 30% its capacity etc. This is done randomly.*/
void floorPed::densityPed(double density) {
	int numberPed = (x - 2) * (y - 2) * density;

	int k = 0;
	int j = 0;

	for (int i = 0; i < numberPed;) {
		k = getRandomInt(1, x);
		j = getRandomInt(1, y);
		pedestrian temp = pedestrian(k, j);
		if (addPed(temp) == 1) {
			i++;
		}
	}

}

/*Fills the floor with pedestrians of the two groups.*/
void floorPed::NEWdensityPed(double density, double paramDensity, double kD2, double kS2) {
	int numberPed = (x - 2) * (y - 2) * density;
	pedGroup1 = numberPed * paramDensity; //Number of pedestrians in the first group
	pedGroup2 = numberPed - pedGroup1; //Number of pedestrians in the second group

	//std::cout << numberPed << std::endl;
	//std::cout << pedGroup1 << std::endl;
	//std::cout << pedGroup2 << std::endl;

	int k = 0;
	int j = 0;

	for (int i = 0; i < pedGroup1;) {
		k = getRandomInt(1, x);
		j = getRandomInt(1, y);
		pedestrian temp = pedestrian(k, j, kD2, kS2, 1);
		if (addPed(temp) == 1) {
			i++;
		}

	}

	for (int i = 0; i < pedGroup2;) {
		k = getRandomInt(1, x);
		j = getRandomInt(1, y);
		pedestrian temp = pedestrian(k, j, kD, kS, 2);
		if (addPed(temp) == 1) {
			i++;
		}
	}
}

/*Updates the dynamic field. With the use of a random number, it will decide if the dynamic field cell will decay or diffuse.
Diffussion is defined as increasing one of its neighbors value by one without diminishing the original cell's value.*/
void floorPed::dynamicDecay() {
	int difI;
	bool passed = false;
	int dynMagn = 0;


	//TODO: Fix coordinate system
	/*Runs the dynamic decay for all cells in the floor.
	Will not run on walls to avoid bleeding of the dynamic field in walls cells to the floor cells.
	Since it will not run on walls, it is probable it wont run on doors.*/
	for (int i = 1; i < x - 1; i++) {
		for (int j = 1; j < y - 1; j++) {
			dynMagn = dynField[i][j];
			for (int l = 0; l < dynMagn; l++) {
				if (getRandom01() <= delta) {
					dynField[i][j] -= 1;
				}
			}
			dynMagn = dynField[i][j];
			for (int m = 0; m < dynMagn; m++) {
				if (getRandom01() <= alpha) {
					//dynField[i][j] -= 1;
					difI = 1 + getRandomInt(0, 2) * (-2);
					if (getRandomInt(0, 2) == 0) {
						auxDynField[i + difI][j] += 1;
					}
					else {
						auxDynField[i][j + difI] += 1;
					}
				}
			}
		}
	}

	/*If the doors are in the walls, which is mostly the case, the dynamic field of them will calculate its diffussion and decay here.*/
#ifdef DOOR_IN_WALL
	int i = 0;
	int j = 0;

	for (int k = 0; k < door.size(); k++) {
		i = door[k][0];
		j = door[k][1];
		dynMagn = dynField[i][j];
		for (int l = 0; l < dynMagn; l++) {
			if (getRandom01() <= delta) {
				dynField[i][j] -= 1;
			}
		}
		dynMagn = dynField[i][j];
		for (int m = 0; m < dynMagn; m++) {
			if (getRandom01() <= alpha) {
				//dynField[i][j] -= 1;

				difI = 1 + getRandomInt(0, 2) * (-2);
				while (passed == false) {
					if (i + difI < 0 || i + difI >= x || j + difI < 0 || j + difI >= y) {
						difI = 1 + getRandomInt(0, 2) * (-2);
					}
					else {
						passed = true;
					}
				}
				passed = false;
				if (getRandomInt(0, 2) == 0) {
					auxDynField[i + difI][j] += 1;
				}
				else {
					auxDynField[i][j + difI] += 1;
				}
			}
		}
	}
#endif


	for (int i = 0; i < x - 1; i++) {
		for (int j = 1; j < y - 1; j++) {
			dynField[i][j] += auxDynField[i][j];
			auxDynField[i][j] = 0;
			if (dynField[i][j] < 0) {
				dynField[i][j] = 0;
			}
		}
	}

}

/*Checks if the pedestrian is standing at the door and "saves" it. This pedestrian is erased from the pedestrian vector.
k refers to the door id.*/
void floorPed::isPedSafe(int k) {
	for (int p = 0; p < pedVec.size();) {
		if (pedVec[p].position == door[k]) {
			occupied.at(pedVec[p].position[0]).at(pedVec[p].position[1]) = 0;
			pedVec.erase(pedVec.begin() + p);
			dynField[door[k][0]][door[k][1]] += 1;
		}
		else {
			p++;
		}
	}
}


/*The probability matrix is linearized and has 5 members,
one for each direction the pedestrian will move. It ignores diagonals, and so follows the von Neumman neighborhood
p is the ID of the pedestrian*/
void floorPed::calcProbVec(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];
	double N = 0;
	double maxFloorValue = 0;
	std::vector<double> floorValues = { 0,0,0,0,0 };
	std::vector<int> correctionCells = { 4,3,2,1,0 };

	int randomInt = getRandomInt(0, 5);

	floorValues[0] = kS * statField[i - 1][j] + kD * dynField[i - 1][j] * (1 - occupied[i - 1][j]) * obstacle[i - 1][j];
	floorValues[1] = kS * statField[i][j - 1] + kD * dynField[i][j - 1] * (1 - occupied[i][j - 1]) * obstacle[i][j - 1];
	floorValues[2] = kS * statField[i][j] + kD * dynField[i][j] * (1 - occupied[i][j]) * obstacle[i][j];
	floorValues[3] = kS * statField[i][j + 1] + kD * dynField[i][j + 1] * (1 - occupied[i][j + 1]) * obstacle[i][j + 1];
	floorValues[4] = kS * statField[i + 1][j] + kD * dynField[i + 1][j] * (1 - occupied[i + 1][j]) * obstacle[i + 1][j];

	maxFloorValue = floorValues[randomInt];

	for (int i = 0; i < floorValues.size(); i++) {
		if (floorValues[i] > maxFloorValue) {
			maxFloorValue = floorValues[i];
		}
	}

	//"North" 
	double n = probFunction(i - 1, j, maxFloorValue);
	//"West"
	double w = probFunction(i, j - 1, maxFloorValue);
	//"Center"
	occupied[i][j] = 0; // This is done so that the probability can be calculated as if the cell wasnt occupied 
	double c = probFunction(i, j, maxFloorValue);
	occupied[i][j] = 1;
	//"East"
	double e = probFunction(i, j + 1, maxFloorValue);
	//"South"
	double s = probFunction(i + 1, j, maxFloorValue);


	/*Correction algorithm. If the pedestrian moved north on its past move, then it left a dynamic field boson in the cell south of it. 
	This boson is erased in order to avoid self-following.*/
	switch (correctionCells[pedVec[p].desiredDirection])
	{
	case 0:
		n = probFunctionCorrection(i - 1, j, maxFloorValue);
		break;
	case 1:
		w = probFunctionCorrection(i, j - 1, maxFloorValue);
		break;
	case 3:
		e = probFunctionCorrection(i, j + 1, maxFloorValue);
		break;
	case 4:
		s = probFunctionCorrection(i + 1, j, maxFloorValue);
		break;
	default:
		break;
	}


	N = 1 / (n + w + c + e + s); //Normalization factor


	pedVec[p].probVec[0] = N * n;
	pedVec[p].probVec[1] = N * w;
	pedVec[p].probVec[2] = N * c;
	pedVec[p].probVec[3] = N * e;
	pedVec[p].probVec[4] = N * s;

}

void floorPed::NEWcalcProbVec(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];

	double N = 0;
	std::cout << "pedVec without normalizing " << std::endl;
	double n = NEWexpFunction(i - 1, j, p);
	double w = NEWexpFunction(i, j - 1, p);
	double c = 1;
	double e = NEWexpFunction(i, j + 1, p);
	double s = NEWexpFunction(i + 1, j, p);


	std::cout << "N: " << n << ", " << "W: " << w << ", " << "C: " << c << ", " << "E: " << e << ", " << "S: " << s << std::endl;

	N = 1 / (n + w + c + e + s);

	pedVec[p].probVec[0] = n * N;
	pedVec[p].probVec[1] = w * N;
	pedVec[p].probVec[2] = c * N;
	pedVec[p].probVec[3] = e * N;
	pedVec[p].probVec[4] = s * N;

}


/*All pedestrians calculate their probability vector and decide which cells they will move to*/
void floorPed::pedDecideVect() {
	for (int p = 0; p < pedVec.size(); p++) {
		calcProbVec(p);
		pedVec[p].chooseMoveVec();
	}
}

/*Runs calcProbVec for all pedestrians and then makes the pedestrian choose their new desired move*/
void floorPed::NEWPedDecide() {
	for (int p = 0; p < pedVec.size(); p++) {
		NEWcalcProbVec(p);
		pedVec[p].chooseMoveVec();

	}
}

/*Conflicts are searched for in the conflictVect and solved.
Uses the conflict vector*/
void floorPed::findNResolveConflicts() {
	double maxProbTemp = 0;
	double NValue = 0;
	double rand = randomNumber_01(5);

	int maxPed = 0;
	int it = 0;
	int jt = 0;

	for (int k = 0; k < conflictVec.size(); k++) {
		if (conflictVec.at(k).size() > 0) {
			/*Will calculate the normalization value for this transition probabilities*/
			for (int p = 0; p < conflictVec.at(k).size(); p++) {
				NValue += conflictVec.at(k).at(p)->probMax;
			}
			for (int p = 0; p < conflictVec.at(k).size(); p++) {
				maxProbTemp += conflictVec.at(k).at(p)->probMax * (1 / NValue);
				if (rand - maxProbTemp < EPSILON / 10) {
					maxPed = p;
					break;
				}
			}
			if (conflictVec.at(k).at(maxPed)->position != conflictVec.at(k).at(maxPed)->desiredMove) {
				//std::cout << "Pedestrian has dropped a crumble to position" << conflictVec.at(k).at(maxPed)->position[0] << ", " << conflictVec.at(k).at(maxPed)->position[1] << std::endl;
				dynField[conflictVec.at(k).at(maxPed)->position[0]][conflictVec.at(k).at(maxPed)->position[1]] += 1;
			}
			occupied.at(conflictVec.at(k).at(maxPed)->position[0]).at(conflictVec.at(k).at(maxPed)->position[1]) = 0;
			conflictVec.at(k).at(maxPed)->position = conflictVec.at(k).at(maxPed)->desiredMove;
			occupied.at(conflictVec.at(k).at(maxPed)->position[0]).at(conflictVec.at(k).at(maxPed)->position[1]) = 1;

			maxProbTemp = 0;
			maxPed = 0;
			rand = randomNumber_01(5);
			NValue = 0;

		}
		conflictVec.at(k).clear();
	}
}

/*The conflict vect is filled with the conflicting pedestrians.
The coordinates are linearized into a vector, and in each index the pedestrians who want to move to that coordinate are added.*/
void floorPed::fillConflictVect() {
	int k = 0;
	for (int p = 0; p < pedVec.size(); p++) {
		k = coord2Indx(p);
		conflictVec[k].push_back(&pedVec[p]);
	}

}

/*New exponential function which will not throw NaN when the static field value is too high.
This one takes the kD and kS of each pedestrian instead of the global parameters. Not used yet, hopefully will be.*/
double floorPed::NEWexpFunction(int i, int j, int p) {

	int x = pedVec[p].position[0];
	int y = pedVec[p].position[1];
	double pedKD = pedVec[p].pedKD;
	double pedKS = pedVec[p].pedKS;

	return (exp(pedKD * (dynField[i][j] - dynField[x][y]))) *
		(exp(pedKS * (statField[i][j] - statField[x][y]))) * ((1 - occupied[i][j])) * (obstacle[i][j]);

}

/*Probability function which returns the transition probability for a pedestrian to cell i,j.
Will return 0 is the cell is occupied or has an obstacle.*/
double floorPed::probFunction(int i, int j, double maxFloorValues) {
	if ((1 - occupied[i][j]) * obstacle[i][j] == 0) {
		return 0;
	}
	else {
		return exp(kD * dynField[i][j] + kS * statField[i][j] - maxFloorValues);
	}

}

/*Probability function which returns the transition probability for a pedestrian to cell i,j.
Will return 0 is the cell is occupied or has an obstacle. This function will decrease the value of the dynamic field in 1 to avoid self-interaction with its own trace.*/
double floorPed::probFunctionCorrection(int i, int j, double maxFloorValues) {
	if ((1 - occupied[i][j]) * obstacle[i][j] == 0) {
		return 0;
	}
	else {
		return exp(kD * (dynField[i][j] - 1) + kS * statField[i][j] - maxFloorValues);
	}
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*____________________ALGORITHM RUNS____________________
Single runs programmed. Works with the pedestrians to calculate their movements.*/



/*New testRun now using the new probability vectors and efficient conflict solving.*/
void floorPed::singleRun() {
	/*The dynamic field is updated with the decay and difussion dynamics.*/
	dynamicDecay();

	/*Pedestrians calculate their probability vector and chooses the highest probability.
	Then, from this probability, they select their desired move.*/
	pedDecideVect();

	/*The ConflictVect is filled up with the pedestrian's desired move to then resolve the conflicts.*/
	fillConflictVect();

	/*If two or more pedestrians have the same desired move,
	the conflicts are resolved by choosing the pedestrian with the highest probability to move.
	It moves the pedestrians and adds 1 to the auxiliary dynamic field. (this assures that the dropped boson will have lifetime of at least 1 before being subject to difussion and decay)*/
	findNResolveConflicts();

	/*Checks which pedestrians are safe. This means, which pedestrians are standing on the door's positions*/

	for (int k = 0; k < door.size(); k++) {
		isPedSafe(k);
	}
}

/*New testRun now using the new probability vectors and efficient conflict solving. WIP
Will be changed from testRun to finalRun, since this is the most updated version of the individual run.*/
void floorPed::singleRunGroupedParameters() {

	resetGroupMatrix();

	/*The dynamic field is updated with the decay and difussion dynamics.*/
	dynamicDecay();

	/*Pedestrians calculate their probability vector and chooses the highest probability.
	Then, from this probability, they select their desired move.*/
	NEWPedDecide();

	/*The ConflictVect is filled up with the pedestrian's desired move to then resolve the conflicts.*/
	fillConflictVect();

	/*If two or more pedestrians have the same desired move,
	the conflicts are resolved by choosing the pedestrian with the highest probability to move.
	It moves the pedestrians and adds 1 to the dynamic field.*/
	findNResolveConflicts();

	/*Checks which pedestrians are safe. This means, which pedestrians are standing on the door's positions*/
	for (int k = 0; k < door.size(); k++) {
		isPedSafe(k);
	}

	groupMatrixFill();

}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*____________________RESETTING FUNCTIONS____________________
Reset the floor and its fields for new runs.*/

/*The floor is reseted and all occupied cells are made 0.*/
void floorPed::resetOccupied() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			occupied[i][j] = 0;
		}
	}
}

/*Resets the floor to an initial condition by:
Erasing all pedestrians (Not needed)
Setting all spaces as unoccupied (Not needed)
Reseting the dynamic field to 0
Filling up the floor with p pedestrians*/
void floorPed::resetFloor(int p) {
	erasePed();
	resetOccupied();
	resetDynField();
	ranPed(p);
}

/*Resets the floor to an initial condition by:
Reseting the dynamic field to 0
Filling up the floor with rho density*/
void floorPed::resetFloor(double rho) {
	erasePed();
	resetDynField();
	resetOccupied();
	densityPed(rho);
}

/*Resets the dynamic field to be 0 on all cells.*/
void floorPed::resetDynField() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			dynField[i][j] = 0;
		}
	}
}

/*Resets the group matrix to be 0 on all cells.*/
void floorPed::resetGroupMatrix() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			groupMatrix[i][j] = 0;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*____________________PRINTING AND WRITING FUNCTIONS____________________
Prints values or arrays to the console or writes them unto text files.*/

/*Writes the occupation field unto a text file*/
void floorPed::writeMovements2File(std::string fileName) {
	std::ofstream file;
	int ped = 0;
	file.open(fileName + ".txt");
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == y - 1) {
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
}

/*Writes the group matrix unto a text file*/
void floorPed::NEWwriteMovements2File(std::string fileName) {
	std::ofstream file;
	int ped = 0;
	file.open(fileName + ".txt");
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == y - 1) {
				file << groupMatrix[i][j];
			}
			else {
				file << groupMatrix[i][j] << ":";
			}

		}
		file << "\n";
	}
	file.close();
}

/*Writes the static field of the floor to a text file*/
void floorPed::writeStatField2File(std::string fileName) {
	std::ofstream file;
	file.open(fileName + ".txt");

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == y - 1) {
				file << statField[i][j];
			}
			else {
				file << statField[i][j] << ":";
			}

		}
		file << "\n";
	}
	file.close();
}

/*Writes the numerical values of the dynamic field to a text file named fileName*/
void floorPed::writeDynField2File(std::string fileName) {

	std::ofstream file;
	file.open(fileName + ".txt");

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (j == y - 1) {
				file << dynField[i][j];
			}
			else {
				file << dynField[i][j] << ":";
			}
		}
		file << "\n";
	}
}

/*Writes the room's dimensions, pedestrian population, alpha, beta, kS, kD and door coordinates
to a text file which is saved in path*/
void floorPed::writeData2File(std::string path) {

	std::ofstream file;
	file.open(path + "/floorData.txt");
	file << "x:" << x << "\n";
	file << "y:" << y << "\n";
	file << "kS:" << kS << "\n";
	file << "kD:" << kD << "\n";
	file << "alpha:" << alpha << "\n";
	file << "delta:" << delta << "\n";
	file << "numbPed:" << pedVec.size() << "\n";
	file << "door_coordinates:";

	for (int i = 0; i < door.size(); i++) {
		file << door[i][0] << "," << door[i][1] << ":";
	}
	file << "\n";
	file.close();
}

/*Prints the occupation field unto the terminal*/
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

/*Prints the dynamic field unto the terminal*/
void floorPed::printDynField() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << dynField[i][j] << ":";
		}
		std::cout << "\n";
	}
}

/*Prints the static field unto the terminal*/
void floorPed::printStatField() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << statField[i][j] << ":";
		}
		std::cout << "\n";
	}
}

/*Prints the value kS on the console.*/
void floorPed::printKs() {
	std::cout << kS << "\n";
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*____________________EDITING FUNCTIONS (GETTER & SETTER) ____________________
Accesses and gives new values to floor variables
or accesses and returns the current floor variables*/

/*Accesses and overwrites the dimensions x and y of the room*/
void floorPed::changeSize(int _x, int _y) {
	x = _x;
	y = _y;
}

/*Accesses and overwrites the value kD of the room*/
void floorPed::changeKD(double _kD) {
	kD = _kD;
}

/*Accesses and returns the value kD of the room*/
double floorPed::getKD() {
	return kD;
}

/*Accesses and overwrites the value kS of the room*/
void floorPed::changeKS(double _kS) {
	kS = _kS;
}

/*Accesses and returns the value kS of the room*/
double floorPed::getKS() {
	return kS;
}

/*Accesses and overwrites the value alpha of the room*/
void floorPed::changeAlpha(double _alpha) {
	alpha = _alpha;
}

/*Accesses and overwrites the value beta of the room*/
void floorPed::changeDelta(double _delta) {
	delta = _delta;
}

/*Returns the number of pedestrians actually inhabiting the floor*/
int floorPed::numberOfPed() {
	return pedVec.size();
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/