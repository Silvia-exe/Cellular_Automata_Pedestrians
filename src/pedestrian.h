#pragma once
#include <vector>
#include <iostream>
#include <algorithm>

class pedestrian {

	friend class floorPed;

	//class variables
	std::vector<int> position; //Will hold two elements only, x and y
	std::vector<std::vector<double>> probMat; //3x3 matrix that show the probability of the pedestrian to move to each cell
	std::vector<double> probVec; //Vector of size 5 which holds the probabilties. 0 is north, 1 is west, 2 is center, 3 is east, 4 is south.
	std::vector<int> desiredMove; //Hold the chosen cell the pedestrian wants to move to. Will be used to detect and deal with conflicts
	double probMax; //Holds the probability of the choosen cell the pedestrian wants to move to
	bool escape;//0 if not at the door, 1 when at the door

	void initializePedestrian(int x, int y) {
		position.push_back(x);
		position.push_back(y);
		desiredMove = { 0,0 };
		escape = 0;
		initProbVec();
		initProbMat();
	}

private:
	void initProbMat();
	void initProbVec(bool diag = 0);

public:
	void move();
	void chooseMove();
	void chooseMoveVec();

	void returnProbMat();
	std::vector<int> returnPosition();

	pedestrian(int x_, int y_) {
		initializePedestrian(x_, y_);
	}

};
