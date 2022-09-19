#pragma once
#include <vector>
#include <iostream>
#include <algorithm>

class pedestrian {

	friend class floorPed;

	//class variables
	std::vector<int> position; //Will hold two elements only, x and y
	std::vector<double> probVec; //Vector of size 5 which holds the probabilties. 0 is north, 1 is west, 2 is center, 3 is east, 4 is south.
	int desiredDirection;
	std::vector<int> desiredMove; //Hold the chosen cell the pedestrian wants to move to. Will be used to detect and deal with conflicts
	double probMax; //Holds the probability of the choosen cell the pedestrian wants to move to
	double pedKD; //Individual KD parameter that affects this individual pedestrian
	double pedKS; //Individual KS parameter that affects this individual pedestrian
	int groupNumber; //Indicates which group this pedestrian belongs to.

	void initializePedestrian(int x, int y, double pedKD_, double pedKS_, int group_) {
		position.push_back(x);
		position.push_back(y);
		desiredMove = {0, 0};
		initProbVec();
	

		pedKD = pedKD_;
		pedKS = pedKS_;
		groupNumber = group_;
		desiredDirection = 2;
	}

private:
	void initProbVec(bool diag = 0);
	int chooseDesiredMove();

public:
	void chooseMoveVec();
	

	std::vector<int> returnPosition();

	pedestrian(int x_, int y_, double pedKD_ = 0.0, double pedKS_ = 0.0, int group_ = 0) {
		initializePedestrian(x_, y_, pedKD_, pedKS_, group_);
	}

};