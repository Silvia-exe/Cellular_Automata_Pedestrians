
#include "stdafx.h"
#include "pedestrian.h"

/*Initiatializes the probability matrix as a 3x3 matrix of zeros*/
void pedestrian::initProbMat() {
	for (int i = 0; i < 3; i++) {
		std::vector<double> temp;
		for (int j = 0; j < 3; j++) {
			temp.push_back(0);
		}
		probMat.push_back(temp);
	}
}

void pedestrian::initProbVec(bool diag) {
	probVec.resize(5 + 4*diag);
}

/*Runs through the probability matrix and selects the cell the pedestrian will move to*/
void pedestrian::move() {
	double maxTemp = 0;
	int newX = 0;
	int newY = 0;
	if (probMat[1][0] + probMat[0][1] + probMat[1][2] + probMat[2][1] == 0) {
		newX = 1;
		newY = 1;
	}
	else {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (probMat[i][j] > maxTemp) {
					maxTemp = probMat[i][j];
					probMax = maxTemp;
					newX = i;
					newY = j;
				}
			}
		}
	}

	position[0] += (newX - 1);
	position[1] += (newY - 1);

}

void pedestrian::chooseMove() {
	double maxTemp = 0;
	int newX = 0;
	int newY = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (probMat[i][j] > maxTemp) {
				maxTemp = probMat[i][j];
				probMax = maxTemp;
				newX = i;
				newY = j;
			}
		}
	}

	desiredMove[0] = position[0] + (newX - 1);
	desiredMove[1] = position[1] + (newY - 1);

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
	double max = 0;
	int j;
	for (int i = 0; i < probVec.size(); i++) {
		if (probVec[i] > max) {
			//std::cout << "entered here" << std::endl;
			j = i;
			max = probVec[i];
		}
	}

	desiredDirection = j;
	probMax = max;

	switch (j){
	case 0:
		desiredMove[0] = position[0] - 1;
		desiredMove[1] = position[1];
		//probMax = *i;
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


void pedestrian::returnProbMat() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << probMat[i][j] << ":";
		}
		std::cout << "\n";
	}
}

std::vector<int> pedestrian::returnPosition() {
	return position;
}

