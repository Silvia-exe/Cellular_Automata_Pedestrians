#define EPSILON 0.00005
#include "stdafx.h"
#include "pedestrian.h"

int randomInt(int min, int max)
{
	return (rand() % (max - min)) + min;
}

/*Gets a random number between 0 and 1 with a selected amount of decimal spaces*/
double randomNumber01(int decimalSpaces, double min = EPSILON) {
	return floor(((double)randomInt(min * RAND_MAX, RAND_MAX) / RAND_MAX) * pow(10, decimalSpaces)) / pow(10, decimalSpaces);
}

void pedestrian::initProbVec(bool diag) {
	probVec.resize(5 + 4*diag);
}



int pedestrian::chooseDesiredMove() {
	double chooseCellRand = randomInt(0, 5);
	double comparison = randomNumber01(5);
	double tempProb = probVec[chooseCellRand];
	bool hasPedChosen = false;

	while (hasPedChosen == false) {
		if (comparison <= tempProb){
			hasPedChosen = true;
			return chooseCellRand;
		}
		else {
			chooseCellRand = randomInt(0, 5);
			comparison = randomNumber01(5);
			tempProb = probVec[chooseCellRand];
		}
	}

}

/*The highest probability of the probVec is found. The index indicates if the pedestrian's desired move is:
0 - North
1 - West
2 - Stays in their initial position
3 - East
4 - South*/
void pedestrian::chooseMoveVec() {
	/*std::vector<double>::iterator i = std::max_element(probVec.begin(), probVec.end());
	int j = std::distance(probVec.begin(), i);
	double maxProb = *i;*/

	chooseDesiredMove();
	
	desiredDirection = chooseDesiredMove();
	probMax = probVec[desiredDirection];

	//std::cout << "--------------------------oooo---------------------" << std::endl;

	switch (desiredDirection){
	case 0:
		desiredMove[0] = position[0] - 1;
		desiredMove[1] = position[1];
		break;
	case 1:
		desiredMove[0] = position[0];
		desiredMove[1] = position[1] - 1;
		break;
	case 2:
		desiredMove = position;
		break;
	case 3:
		desiredMove[0] = position[0];
		desiredMove[1] = position[1] + 1;
		break;
	case 4:
		desiredMove[0] = position[0] + 1;
		desiredMove[1] = position[1];
		break;
	}

}

std::vector<int> pedestrian::returnPosition() {
	return position;
}



