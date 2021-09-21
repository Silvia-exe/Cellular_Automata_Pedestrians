#include "stdafx.h"
#include "floorPed.h"

/*Fills the matrixes with 0 in order to initialize them.
Makes indexing easier*/
void floorPed::startMat() {
	std::vector<std::vector<double>> temp;
	for (int i = 0; i < x; i++) {
		std::vector<double> doubTemp;
		std::vector<int> intTemp;
		for (int j = 0; j < y; j++) {
			doubTemp.push_back(0);
			intTemp.push_back(0);
		}
		dynField.push_back(doubTemp);
		statField.push_back(doubTemp);
		occupied.push_back(intTemp);
		obstacle.push_back(intTemp);
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

/*Initializes the conflict vector. It is a vector of vectors which holds the pedestrian's desired move. It will have x*y pedestrian vectors*/
void floorPed::initConflictVec() {
	std::vector<pedestrian*> temp;
	for (int i = 0; i < x*y; i++) {
		conflictVec.push_back(temp);
	}
	std::cout << "conflictVect size: " << conflictVec.size() << std::endl;
}

/*Fills up the floor with n number of randomly placed pedestrians*/
void floorPed::ranPed(int n) {

	
	if (n >= (x - 2)*(y - 2)) {
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
	int numberPed = (x - 2)*(y - 2) * density;

	std::cout << numberPed << " are supossed to be added" << std::endl;

	if (density >= 1) {
		std::cout << "Density is too high" << "\n";
	}

	int k = 0;
	int j = 0;
	
	for (int i = 0; i < numberPed; i++) {
		k = rand() % x;
		j = rand() % y;
		pedestrian temp = pedestrian(k, j);
		if (addPed(temp) == 0) {
			i--;
		}/*else{ std::cout << "Pedestrian " << i << " added at " << temp.position[0] << ", " << temp.position[1] << std::endl; }*/
		
	}

}

/*Adds a pedestrian to the floor if the cell isn't occupied or has an obstacle*/
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

/*Calculates the furthest away cell from every door*/
void floorPed::calcDL() {
	//std::cout << "Entered calcDL" << "\n";
	std::vector<int> d(door.size());
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < door.size(); k++) {
				d[k] = sqrt(((door[k][0] - i)*(door[k][0] - i)) + ((door[k][1] - j)*(door[k][1] - j)));
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
				statFieldVect[k][i][j] = d_L[k] - sqrt((door[k][0] - i)*(door[k][0] - i) + (door[k][1] - j)*(door[k][1] - j));
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
	//printStatField();
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

/*Prints the static field on the console*/
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

int inline getRandom(int min, int max)
{
	return (rand() % (max - min)) + min;
}


/*Updates the dynamic field. With the use of a random number, it will decide if the dynamic field cell will decay.
If it decays, it will also decide if it diffuses or not*/
void floorPed::dynamicDecay() {

	double difI;
	double maxDynValInv = 0;
	bool passed = false;

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (obstacle.at(i).at(j) == 0) {
				if (dynField[i][j] != 0) {
					if (dynField[i][j] >= maxDynValInv) {
						maxDynValInv = dynField[i][j];
					}

					if (getRandom(0, 10) * (0.1) < beta) {
						dynField[i][j] -= 1;

						if (getRandom(0, 10) * (0.1) < alpha) {
							difI = getRandom(-1, 1);
							
							while (passed == false) {
								
								try
								{
									if (dynField.at(i + difI).at(j) && dynField.at(i).at(j + difI)) {

										std::cout << "PASSED " << std::endl;
										std::cout << difI << std::endl;
										passed = true;

									}
									else {
										difI = getRandom(-1, 1);
										std::cout << difI << std::endl;
										throw 404;

									}
									
								}
								catch (const std::exception&)
								{
									std::cout << i + difI << ", " << j << std::endl;
									std::cout << i << ", " << j+difI << std::endl;
									std::cout << "Error" << std::endl;
				
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
	}

	for (int i = 0; i < x ; i++) {
		std::transform(dynField[i].begin(), dynField[i].end(), dynField[i].begin(), [maxDynValInv](double &c) { return c * (1/maxDynValInv); });
	}
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

/*Prints the value kS on the console. Probably a silly function which will be deleted.*/
void floorPed::printKs() {
	std::cout << kS << "\n";
}

/*Resets the dynamic field to be 0 on all cells.*/
void floorPed::resetDynField() {

	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {

			dynField[i][j] = 0;

		}
	}
}

/*Clears the whole pedestrian vector. Silly function.*/
void floorPed::erasePed() {
	pedVec.clear();
}

/*Checks if the pedestrian is standing at the door and "saves" it. This pedestrian is erased from the pedestrian vector.*/
void floorPed::isPedSafe(int p) {
	for (int i = 0; i < door.size(); i++) {
		if (pedVec[p].position == door[i]) {
			//std::cout << "Pedestrian " << p << " was saved" << std::endl;
			occupied.at(pedVec[p].position[0]).at(pedVec[p].position[1]) = 0;
			pedVec[p].escape = 1;
			pedVec.erase(pedVec.begin() + p);
			//std::cout << pedVec.size() << " pedestrians remaining" << std::endl;
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

	N = 1 / (n+w+c+e+s);

	pedVec[p].probVec[0] = n * N;
	pedVec[p].probVec[1] = w * N;
	pedVec[p].probVec[2] = c * N;
	pedVec[p].probVec[3] = e * N;
	pedVec[p].probVec[4] = s * N;

}


/*Exponential function that is the basis for calculating the probability matrix DEPRECATED*/
double floorPed::expFunction(int i, int j) {

	return exp(kD * dynField[i][j])*exp(kS*statField[i][j])*(1 - occupied[i][j])*obstacle[i][j];

}

/*New exponential function which will not throw NaN when the static field value is too high.*/
double floorPed::expFunction(int i, int j, int p) {

	int x = pedVec[p].position[0];
	int y = pedVec[p].position[1];
	long double expRes = long double(exp(kD * (dynField[i][j] - dynField[x][y]))) * long double(exp(kS * (statField[i][j] - statField[x][y]))) * long double((1 - occupied[i][j])) * long double(obstacle[i][j]);
	
	if (isnan(expRes) || isinf(expRes)) {
		std::cout << "_______________________________________________" << std::endl;
		std::cout << isnan(expRes) << " ," << isinf(expRes) << std::endl;
		std::cout << ".....Problem detected....." << std::endl;
		std::cout << p << std::endl;

		std::cout << "exp(" << kD << "*(" << dynField[i][j] << "-" << dynField[x][y] << ")) = " << exp(kD * (dynField[i][j] - dynField[x][y])) << std::endl;
		std::cout << "exp(" << kS << "*(" << statField[i][j] << "-" << statField[x][y] << ")) = " << exp(kD * (statField[i][j] - statField[x][y])) << std::endl;
		std::cout << " 1 - " << occupied[i][j] << "= " << 1-occupied[i][j] << std::endl;
		std::cout << obstacle[i][j] << std::endl;
		return 10000;
	}
	
	return exp(kD * (dynField[i][j] - dynField[x][y]))*exp(kS*(statField[i][j] - statField[x][y]))*(1-occupied[i][j])*obstacle[i][j];

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

/*New testRun now using the new probability vectors and efficient conflict solving. WIP*/
void floorPed::testRun() {
	//std::cout << "Entered test Run " << std::endl;
	for (int p = 0; p < pedVec.size(); p++) {
		isPedSafe(p);
	}

	//std::cout << "Passed Ped Safe " << std::endl;
	newPedDecide();
	//std::cout << "Passed Ped Decide " << std::endl;
	fillConflictVect();
	//std::cout << "Conflict Vect " << std::endl;
	newFindNResolveConflicts();
	//std::cout << "Passed Resolve Confl " << std::endl;
	dynamicDecay();
	//std::cout << "Passed Dyn Decay " << std::endl;

	//std::cout << "-------------End Test Run--------------" << std::endl;
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

/**/
void floorPed::pedDecideDiag() {
	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 0) {
			calcProbMatDiag(p);
			pedVec[p].chooseMove();
		}
	}
}

/*Im not sure why this is written. It makes all spaces of the pedestrian's probability matrix -1. DEPRECATED I guess*/
void floorPed::resetSavedPed(int p) {
	pedVec[p].desiredMove = { -1,-1 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			pedVec[p].probMat[i][j] = -1;
		}
	}
}


/*Converts the desired move of the "loser" in its actual position, so the "loser" wont move,
and the "winner" will. */
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


void floorPed::newPedDecide() {
	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 0) {
			calcProbVec(p);
			pedVec[p].chooseMoveVec();

			/*std::cout << "Pedestrian " << p << " has choosen " << std::endl;
			std::cout << "Position: " << pedVec[p].position[0] << ", " << pedVec[p].position[1] << std::endl;
			for (int i = 0; i < 5; i++) {
				std::cout << pedVec[p].probVec[i] << ",";
			}
			std::cout << std::endl;
			std::cout << "Max prob: " << pedVec[p].probMax << std::endl;
			std::cout << std::endl;*/
		}

	}
}

void floorPed::fillConflictVect() {
	int k = 0;
	for (int p = 0; p < pedVec.size(); p++) {
		if (pedVec[p].escape == 0) {
			k = coord2Indx(p);
			conflictVec[k].push_back(&pedVec[p]);
		}

	}
}

int floorPed::coord2Indx(int p) {
	int i = pedVec[p].desiredMove[0];
	int j = pedVec[p].desiredMove[1];
	return j + i * (x - 1);
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

void floorPed::resetFloor(int p) {
	erasePed();
	resetOccupied();
	resetDynField();
	ranPed(p);
}

void floorPed::resetFloor(double rho) {
	erasePed();
	resetOccupied();
	resetDynField();
	densityPed(rho);
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

int floorPed::numberOfPed() {
	return pedVec.size();
}

void floorPed::printDynField() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			std::cout << dynField[i][j] << ":";
		}
		std::cout << "\n";
	}
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

void floorPed::newFindNResolveConflicts() {
	double maxProb = 0;
	int maxPed = 0;
	int it = 0;
	int jt = 0;
	for (int k = 0; k < conflictVec.size(); k++) {
		//std::cout << "Enter" << std::endl;
		if (conflictVec.at(k).size() > 0) {
			for (int p = 0; p < conflictVec.at(k).size(); p++) {
				if (conflictVec.at(k).at(maxPed)->probMax > maxProb && conflictVec.at(k).at(maxPed)->escape == 0) {
					maxProb = conflictVec.at(k).at(p)->probMax;
					maxPed = p;
				}

				if (conflictVec.at(k).at(maxPed)->position != conflictVec.at(k).at(maxPed)->desiredMove) {
					dynField[conflictVec.at(k).at(maxPed)->desiredMove[0]][conflictVec.at(k).at(maxPed)->desiredMove[1]] += 1;
				}
				occupied.at(conflictVec.at(k).at(maxPed)->position[0]).at(conflictVec.at(k).at(maxPed)->position[1]) = 0;
				conflictVec.at(k).at(maxPed)->position = conflictVec.at(k).at(maxPed)->desiredMove;
				occupied.at(conflictVec.at(k).at(maxPed)->position[0]).at(conflictVec.at(k).at(maxPed)->position[1]) = 1;
			}
			maxProb = 0;
			maxPed = 0;
		}
		//std::cout << "Leave" << std::endl;
		conflictVec.at(k).clear();
	}
}