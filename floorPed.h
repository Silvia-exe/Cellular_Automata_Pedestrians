#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "pedestrian.h"


class floorPed {
	
	friend class pedestrian;

	std::vector<std::vector<double>> dynField; //Declaration of Dynamic Field
	std::vector<std::vector<double>> statField; //Declaration of Static Field
	std::vector<std::vector<std::vector<double>>> statFieldVect; //Vector which holds the static field for every door
	std::vector<std::vector<bool>> occupied; //Declaration of occupation Matrix
	std::vector<std::vector<bool>> obstacle; //Declaration of obstacle Matrix
	std::vector<pedestrian> pedVec; //Vector which holds all pedestrians in the Floor
	
	/*It is noted that for x and y sizes of the room, the number of cells which the pedestrians can move is not xy but
	(x-2)*(y-2) since we take into account the thickness of the walls*/
	
	int x; //X size of the Floor
	int y; // Y size of the Floor
	double kS; //Static sensitivity coefficient 
	double kD; //Dynamic sensitivity coefficient
	double alpha; //Diffuse factor for the Dynamic Field
	double beta; //Decay factor for Dynamic Field
	std::vector<std::vector<int>> door; //Vector which holds doors (vectors with x and y coordinates)
	std::vector<double> d_L; //Holds the cell furthest away from every door

	void initializeFloor(int x_, int y_,double kS_, double kD_, double alpha_ , double beta_,  std::vector<std::vector<int>> door_) {
		
		x = x_;
		y = y_;
		kS = kS_;
		kD = kD_;
		alpha = alpha_;
		beta = beta_;
		door = door_;
		
		startMat();
		initMat();
		//buildWall();
	}

private:

	double expFunction(int i, int j);
	void dynamicDecay();
	void resetOccupied();
	void initMat();
	void startMat();
	void buildWall();
	void calcDL();
	void calcStatF();
	void statFieldInit();
	void calcProbMat(pedestrian& p1);
	void isPedSafe(int p);
	void clearPed(int p);
	
	//void checkConflicts();

public:
	
	bool addPed(pedestrian p1);
	void singleRun();
	void printMovements();
	void printStatField();
	int numberOfPed();
	void singleRunSave();
	int numberOfSavedPed();
	void writeMovements2File(std::string fileName);
	void writeStatField2File(std::string fileName);

	floorPed(int x_, int y_, double kS_, double kD_, double alpha_, double beta_, std::vector<std::vector<int>> door_) {
		initializeFloor(x_, y_, kS_, kD_, alpha_, beta_, door_);
	}
};
