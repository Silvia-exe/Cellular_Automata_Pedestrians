#pragma once
#include <vector>
#include <iostream>

class pedestrian {
	
	friend class floorPed;

	//class variables
	std::vector<int> position; //Will hold two elements only, x and y
	std::vector<std::vector<double>> probMat; //3x3 matrix that show the probability of the pedestrian to move to each cell
	bool escape; //0 if not at the door, 1 when at the door

	void initializePedestrian(int x, int y) {
		position.push_back(x);
		position.push_back(y);
		escape = 0;
		initProbMat();
	}

public:
	
	void move();
	void initProbMat();
	void returnProbMat();
	std::vector<int> returnPosition();
	

	pedestrian(int x_, int y_) {
		initializePedestrian(x_, y_);
	}

};
