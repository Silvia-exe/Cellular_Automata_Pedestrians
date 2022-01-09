#include "stdafx.h"
#include "floorPed.h"

/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*____________________UTILITY FUNCTIONS____________________
Basic tools used by the class*/

/*Gets a random integer between min and max*/
int getRandom(int min, int max) {
	return (rand() % (max - min)) + min;
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

/*Fills the group matrix with the group of each pedestrian, makes following the behaviour of each group easier*/
void floorPed::groupMatrixFill(){
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
	//std::cout << "Entered calcDL" << "\n";
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
	//std::cout << "Entered calcStatF" << "\n";
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
	//std::cout << "Entered statFieldInit" << "\n";
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
	/*Normalizing the static field so that it holds  values between 0 and 1*/
	statFieldNorm();
}

/*Sums all values of the static field in N.
All values of the static field are divided by N (and x*y) DEPRECATED*/
void floorPed::statFieldNorm() {
	double max = 0;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (statField[i][j] >= max) {
				max = statField[i][j];
			}
		}
	}
	double norm = 1 / max;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			statField[i][j] *= norm;
		}
	}
}

/*Normalizes the dynamic field by summing over it and dividing every cell by the norm
DEPRECATED*/
void floorPed::dynFieldNorm() {
	double max = 0;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (dynField[i][j] >= max) {
				max = dynField[i][j];
			}
		}
	}

	double norm = 1 / max;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			dynField[i][j] *= norm;
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
		k = getRandom(1,x-1);
		j = getRandom(1,y-1);
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
		k = getRandom(1, x - 1);
		j = getRandom(1, y - 1);
		pedestrian temp = pedestrian(k, j, kD2,kS2, 1);
		if (addPed(temp) == 1) {
			i++;
		}

	}

	for (int i = 0; i < pedGroup2;) {
		k = getRandom(1, x - 1);
		j = getRandom(1, y - 1);
		pedestrian temp = pedestrian(k, j, kD, kS, 2);
		if (addPed(temp) == 1) {
			i++;
		}
	}
}

/*Updates the dynamic field. With the use of a random number, it will decide if the dynamic field cell will decay.
If it decays, it will also decide if it diffuses or not*/
void floorPed::dynamicDecay() {
	double difI;
	maxDynVal = 0;
	bool passed = false;
	for (int i = 1; i < x - 1; i++) {
		for (int j = 1; j < y - 1; j++) {
			if (dynField[i][j] != 0) {
				if (dynField[i][j] >= maxDynVal) {
					maxDynVal = dynField[i][j];
				}
				if (getRandom(0, 10) * (0.1) <= beta) {
					dynField[i][j] -= 1;
					if (getRandom(0, 10) * (0.1) <= alpha) {
						difI = getRandom(-1, 1);
						while (passed == false) {
							if (i + difI < 0 || i + difI > x || j + difI < 0 || j + difI > y) {
								difI = getRandom(-1, 1);
							}
							else {
								passed = true;
							}
						}
						if (getRandom(0, 1) == 0) {
							dynField[i + difI][j] += 1;
						}
						else {
							dynField[i][j + difI] += 1;
						}
					}
				}
				if (dynField[i][j] < 0) {
					dynField[i][j] = 0;
				}
			}
		}
	}
	if (maxDynVal == 0) {
		std::cout << "Pedestrians are stuck, dynamic field has fully decayed. " << std::endl;
		exit(400);
	}
}

/*Checks if the pedestrian is standing at the door and "saves" it. This pedestrian is erased from the pedestrian vector.*/
void floorPed::isPedSafe(int k) {
	for (int p = 0; p < pedVec.size();) {
		if (pedVec[p].position == door[k]) {
			//std::cout << "Pedestrian # " << p << " has attempted/succesfully escaped" << std::endl;
			occupied.at(pedVec[p].position[0]).at(pedVec[p].position[1]) = 0;
			//pedVec[p].escape = 1;
			//savedPed++;
			pedVec.erase(pedVec.begin() + p);
		}
		else {
			p++;
		}
	}
}

/*Calculates the probability matrix of a given pedestrian. Ignores diagonals. DEPRECATED*/
void floorPed::calcProbMat(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];
	double N = 0;

	pedVec[p].probMat[0][0] = pedVec[p].probMat[2][0] = pedVec[p].probMat[0][2] = pedVec[p].probMat[2][2] = 0;

	double w = expFunction(i, j - 1, p);
	double n = expFunction(i - 1, j, p);
	double e = expFunction(i, j + 1, p);
	double s = expFunction(i + 1, j, p);

	/*With the new probability function, the cell where the pedestrian stands will always have a value of 1*/
	double c = 1;

	N = w + e + n + s + c;
	double norm = 1 / N;

	pedVec[p].probMat[1][0] = (1 / N) * w;
	pedVec[p].probMat[0][1] = (1 / N) * n;
	pedVec[p].probMat[1][2] = (1 / N) * e;
	pedVec[p].probMat[2][1] = (1 / N) * s;
	pedVec[p].probMat[1][1] = (1 / N) * c;


}

/*Calculates the probability matrix of a given pedestrian. Does not ignore diagonals. DEPRECATED*/
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

}

/*Alternative to calcProbMat. The probability matrix is linearized and has 5 members,
one for each direction the pedestrian will move. It ignores diagonals, and so follows the von Neumman neighborhood*/
void floorPed::calcProbVec(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];

	double N = 0;
	//std::cout << "pedVec without normalizing " << std::endl;
	double n = expFunction(i - 1, j, p);
	double w = expFunction(i, j - 1, p);
	double c = 1;
	double e = expFunction(i, j + 1, p);
	double s = expFunction(i + 1, j, p);


	//std::cout << n << ", " << w << ", " << c << ", " << e << ", " << s << std::endl;

	N = 1 / (n + w + c + e + s);

	pedVec[p].probVec[0] = n * N;
	pedVec[p].probVec[1] = w * N;
	pedVec[p].probVec[2] = c * N;
	pedVec[p].probVec[3] = e * N;
	pedVec[p].probVec[4] = s * N;

}

void floorPed::NEWcalcProbVec(int p) {
	int i = pedVec[p].position[0];
	int j = pedVec[p].position[1];

	double N = 0;
	//std::cout << "pedVec without normalizing " << std::endl;
	double n = NEWexpFunction(i - 1, j, p);
	double w = NEWexpFunction(i, j - 1, p);
	double c = 1;
	double e = NEWexpFunction(i, j + 1, p);
	double s = NEWexpFunction(i + 1, j, p);


	//std::cout << n << ", " << w << ", " << c << ", " << e << ", " << s << std::endl;

	N = 1 / (n + w + c + e + s);

	pedVec[p].probVec[0] = n * N;
	pedVec[p].probVec[1] = w * N;
	pedVec[p].probVec[2] = c * N;
	pedVec[p].probVec[3] = e * N;
	pedVec[p].probVec[4] = s * N;

}

/*Exponential function that is the basis for calculating the probability matrix DEPRECATED*/
double floorPed::expFunction(int i, int j) {

	return exp(kD * dynField[i][j]) * exp(kS * statField[i][j]) * (1 - occupied[i][j]) * obstacle[i][j];

}

/*New exponential function which will not throw NaN when the static field value is too high.*/
double floorPed::expFunction(int i, int j, int p) {

	int x = pedVec[p].position[0];
	int y = pedVec[p].position[1];

	return (exp(kD * (dynField[i][j] - dynField[x][y]) / maxDynVal)) *
		(exp(kS * (statField[i][j] - statField[x][y]))) * ((1 - occupied[i][j])) * (obstacle[i][j]);

}

/*New exponential function which will not throw NaN when the static field value is too high.
This one takes the kD and kS of each pedestrian instead of the global parameters.*/
double floorPed::NEWexpFunction(int i, int j, int p) {

	int x = pedVec[p].position[0];
	int y = pedVec[p].position[1];
	double pedKD = pedVec[p].pedKD;
	double pedKS = pedVec[p].pedKS;

	return (exp(pedKD * (dynField[i][j] - dynField[x][y]) / maxDynVal)) *
		(exp(pedKS * (statField[i][j] - statField[x][y]))) * ((1 - occupied[i][j])) * (obstacle[i][j]);

}

/*Function that:
1) calculates the probability matrix of the pedestrians
2) "empties" the floor
3) The pedestrians choose their next move and either take it or stay standing
4) Pedestrians are "added" back to the floor
5) The dynamic field is updated
NOTE: The pedestrians still arent saved in this function
DEPRECATED, used for testing*/
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

/*Function that:
1) calculates the probability matrix of the pedestrians
2) "empties" the floor
3) The pedestrians choose their next move and either take it or stay standing
4) Pedestrians are "added" back to the floor
5) The dynamic field is updated
NOTE: The pedestrians still are saved
DEPRECATED, used for testing*/
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

/*Implemented parallel movement of pedestrians, DEPRECATED*/
void floorPed::singleRunAllTogether() {

	pedDecide();

	for (int p = 0; p < pedVec.size(); p++) {
		findNResolveConflicts(p);
		isPedSafe(p);
	}

	for (int p = 0; p < pedVec.size(); p++) {

		if (pedVec[p].escape == 1) {
			//std::cout << "entered escape if" << "\n";
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

/*Calculates the diagonal probability matrix and allows pedestrians to move in the Moore neighborhood.
DEPRECATED. Has no dynamic Decay*/
void floorPed::singleRunDiag() {

	for (int p = 0; p < pedVec.size(); p++) {
		isPedSafe(p);
	}

	pedDecideDiag();

	for (int p = 0; p < pedVec.size(); p++) {
		findNResolveConflicts(p);
	}

	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 1) {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			resetSavedPed(p);
		}
		else {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			pedVec[p].position = pedVec[p].desiredMove;
			isPedSafe(p);
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 1;
		}

	}
}

/*Final function. Will simulate accordingly. Still uses probability matrix and solves conflicts via the old findNResolveConflicts.
Will be deprecated soon. Does not allow diagonal movements.*/
void floorPed::singleRunDynFieldVonNe() {
	for (int p = 0; p < pedVec.size(); p++) {
		isPedSafe(p);
	}

	pedDecide();

	for (int p = 0; p < pedVec.size(); p++) {
		findNResolveConflicts(p);
	}

	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 1) {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			resetSavedPed(p);
		}
		else {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			if (pedVec[p].position != pedVec[p].desiredMove) {
				dynField[pedVec[p].position[0]][pedVec[p].position[1]] += 1;
			}
			pedVec[p].position = pedVec[p].desiredMove;
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 1;
		}
	}

	dynamicDecay();
}

/*Final function. Will simulate accordingly. Still uses probability matrix and solves conflicts via the old findNResolveConflicts.
Will be deprecated soon. Does allow diagonal movements.*/
void floorPed::singleRunDynFieldMoore() {

	for (int p = 0; p < pedVec.size(); p++) {
		isPedSafe(p);
	}

	pedDecideDiag();

	for (int p = 0; p < pedVec.size(); p++) {
		findNResolveConflicts(p);
	}

	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 1) {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			resetSavedPed(p);
		}
		else {
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 0;
			if (pedVec[p].position != pedVec[p].desiredMove) {
				dynField[pedVec[p].position[0]][pedVec[p].position[1]] += 1;
			}
			pedVec[p].position = pedVec[p].desiredMove;
			occupied[pedVec[p].position[0]][pedVec[p].position[1]] = 1;
		}
	}

	dynamicDecay();

}

/*New testRun now using the new probability vectors and efficient conflict solving. WIP
Will be changed from testRun to finalRun, since this is the most updated version of the individual run.*/
void floorPed::testRun() {
	
	/*Checks which pedestrians are safe. This means, which pedestrians are standing on the door's positions*/
	
	for (int k = 0; k < door.size(); k++) {
		isPedSafe(k);
	}

	/*Pedestrians calculate their probability vector and chooses the highest probability. 
	Then, from this probability, they select their desired move.*/
	pedDecideVect();

	/*The ConflictVect is filled up with the pedestrian's desired move to then resolve the conflicts.*/
	fillConflictVect();

	/*If two or more pedestrians have the same desired move,
	the conflicts are resolved by choosing the pedestrian with the highest probability to move.
	It moves the pedestrians and adds 1 to the dynamic field.*/
	newFindNResolveConflicts();

	/*The dynamic field is updated with the decay and difussion dynamics.*/
	dynamicDecay();

}

/*New testRun now using the new probability vectors and efficient conflict solving. WIP
Will be changed from testRun to finalRun, since this is the most updated version of the individual run.*/
void floorPed::NEWtestRun() {

	resetGroupMatrix();

	/*Checks which pedestrians are safe. This means, which pedestrians are standing on the door's positions*/
	for (int k = 0; k < door.size(); k++) {
		isPedSafe(k);
	}
	
	/*Pedestrians calculate their probability vector and chooses the highest probability.
	Then, from this probability, they select their desired move.*/
	NEWPedDecide();

	/*The ConflictVect is filled up with the pedestrian's desired move to then resolve the conflicts.*/
	fillConflictVect();

	/*If two or more pedestrians have the same desired move,
	the conflicts are resolved by choosing the pedestrian with the highest probability to move.
	It moves the pedestrians and adds 1 to the dynamic field.*/
	newFindNResolveConflicts();
	
	/*The dynamic field is updated with the decay and difussion dynamics.*/
	dynamicDecay();

	groupMatrixFill();

}

/*All pedestrians calculate their probability matrix, and from there they will choose which cell they will move to.*/
void floorPed::pedDecide() {
	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 0) {
			calcProbMat(p);
			pedVec[p].chooseMove();
		}
	}
}

/*All pedestrians calculate their probability matrix, and from there they will choose which cell they will move to.
Takes into consideration diagonals*/
void floorPed::pedDecideDiag() {
	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 0) {
			calcProbMatDiag(p);
			pedVec[p].chooseMove();
		}
	}
}

/*All pedestrians calculate their probability vector and decide which cells they will move to
Newest and best function.*/
void floorPed::pedDecideVect(){
	for (int p = 0; p < pedVec.size(); p++) {
		calcProbVec(p);
		pedVec[p].chooseMoveVec();
	}
}

/*Converts the desired move of the "loser" in its actual position, so the "loser" wont move,
and the "winner" will. DEPRECATED*/
void floorPed::findNResolveConflicts(int p) {
	for (int j = 0; j < pedVec.size(); j++) {
		if (j != p && pedVec[p].escape == 0 && pedVec[j].escape == 0) {
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
					pedVec[p].desiredMove = pedVec[p].position;
				}
			}
		}
	}
}

/*Conflicts are searched for in the conflictVect and solved.
Uses the conflict vector*/
void floorPed::newFindNResolveConflicts() {
	double maxProb = 0;
	int maxPed = 0;
	int it = 0;
	int jt = 0;
	for (int k = 0; k < conflictVec.size(); k++) {
		//std::cout << "Enter" << std::endl;
		if (conflictVec.at(k).size() > 0) {
			for (int p = 0; p < conflictVec.at(k).size(); p++) {
				if (conflictVec.at(k).at(p)->probMax >= maxProb) {
					if (p > 1 && conflictVec.at(k).at(p)->probMax == maxProb) {
						if (getRandom(0, 1) == 0) {
							maxProb = conflictVec.at(k).at(p)->probMax;
							maxPed = p;
						}
					}
					else {
						maxProb = conflictVec.at(k).at(p)->probMax;
						maxPed = p;
					}
					maxProb = conflictVec.at(k).at(p)->probMax;
					maxPed = p;
				}
			}

			if (conflictVec.at(k).at(maxPed)->position != conflictVec.at(k).at(maxPed)->desiredMove) {
				dynField[conflictVec.at(k).at(maxPed)->desiredMove[0]][conflictVec.at(k).at(maxPed)->desiredMove[1]] += 1;
			}
			occupied.at(conflictVec.at(k).at(maxPed)->position[0]).at(conflictVec.at(k).at(maxPed)->position[1]) = 0;
			conflictVec.at(k).at(maxPed)->position = conflictVec.at(k).at(maxPed)->desiredMove;
			occupied.at(conflictVec.at(k).at(maxPed)->position[0]).at(conflictVec.at(k).at(maxPed)->position[1]) = 1;

			maxProb = 0;
			maxPed = 0;
		}
		conflictVec.at(k).clear();
	}
}

/*Runs calcProbVec for all pedestrians and then makes the pedestrian choose their new desired move*/
void floorPed::NEWPedDecide() {
	for (int p = 0; p < pedVec.size(); p++) {
		NEWcalcProbVec(p);
		pedVec[p].chooseMoveVec();	

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
	resetDynField();
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
	file << "beta:" << beta << "\n";
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

/*Prints the static field on the console*/
void floorPed::printStatField() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << statField[i][j] << ":";
		}
		std::cout << "\n";
	}
}

/*Prints the value kS on the console. Probably a silly function which will be deleted.*/
void floorPed::printKs() {
	std::cout << kS << "\n";
}

/*Accesses and overwrites the dimensions x and y of the room*/
void floorPed::changeSize(int _x, int _y) {
	x = _x;
	y = _y;
}

/*Accesses and overwrites the value kD of the room*/
void floorPed::changeKD(double _kD) {
	kD = _kD;
}

/*Accesses and overwrites the value kS of the room*/
void floorPed::changeKS(double _kS) {
	kS = _kS;
}

/*Accesses and overwrites the value alpha of the room*/
void floorPed::changeAlpha(double _alpha) {
	alpha = _alpha;
}

/*Accesses and overwrites the value beta of the room*/
void floorPed::changeBeta(double _beta) {
	beta = _beta;
}

/*Returns the number of pedestrians actually inhabiting the floor*/
int floorPed::numberOfPed() {
	return pedVec.size();
}

/*Returns the number of saved pedestrians DEPRECATED*/
int floorPed::numberOfSavedPed() {
	int savedPed = 0;
	for (int i = 0; i < pedVec.size(); i++) {
		if (pedVec[i].escape == 1) {
			savedPed++;
		}
	}
	return savedPed;
}

/*Clears the whole pedestrian vector. Silly function.*/
void floorPed::erasePed() {
	pedVec.clear();
}

/*Im not sure why this is written. It makes all spaces of the pedestrian's probability matrix -1. DEPRECATED*/
void floorPed::resetSavedPed(int p) {
	pedVec[p].desiredMove = { -1,-1 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			pedVec[p].probMat[i][j] = -1;
		}
	}
}
