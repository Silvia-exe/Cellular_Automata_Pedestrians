#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <chrono>
#include <ctime>
#include <random>
#include "pedestrian.h"

using namespace std::chrono;

class floorPed {

	friend class pedestrian;

	std::vector<std::vector<double>> dynField; //Declaration of Dynamic Field
	std::vector<std::vector<double>> auxDynField; //Declaration of Dynamic Field
	std::vector<std::vector<double>> statField; //Declaration of Static Field
	std::vector<std::vector<bool>> occupied; //Declaration of occupation Matrix
	std::vector<std::vector<bool>> obstacle; //Declaration of obstacle Matrix
	std::vector<std::vector<int>> groupMatrix; //Declaration of group Matrix

	std::vector<std::vector<std::vector<double>>> statFieldVect; //Vector which holds the static field for every door
	std::vector<pedestrian> pedVec; //Vector which holds all pedestrians in the Floor

	std::vector<int> correctionCells = { 4,3,2,1,0 };

	std::vector<std::vector<pedestrian*>> conflictVec;

	/*It is noted that for x and y sizes of the room, the number of cells which the pedestrians can move is not xy but
	(x-2)*(y-2) since we take into account the thickness of the walls*/

	int x; //X size of the Floor
	int y; // Y size of the Floor
	int savedPed = 0; //Counts the number of saved pedestrians DEPRECATED
	double kS; //Static sensitivity coefficient 
	double kD; //Dynamic sensitivity coefficient
	double alpha; //Diffuse factor for the Dynamic Field
	double delta; //Decay factor for Dynamic Field
	int maxDynVal; //Maximum value that the dynamic field can have.
	std::vector<std::vector<int>> door; //Vector which holds doors (vectors with x and y coordinates)
	std::vector<double> d_L; //Holds the distance of the cell furthest away from every door

	int pedGroup1; //Counts the number of pedestrians to hold the first combination of parameters.
	int pedGroup2; //Counts the number of pedestrians to hold the second combination of parameters.

	void initializeFloor(int x_, int y_, double kS_, double kD_, double alpha_, double delta_ ,std::vector<std::vector<int>> door_, int _maxDynVal = 100) {
		x = x_;
		y = y_;
		kS = kS_;
		kD = kD_;
		alpha = alpha_;
		delta = delta_;
		door = door_;
	
		pedGroup1 = 0;
		pedGroup2 = 0;

		maxDynVal = _maxDynVal;

		startMat();
		initMat();
		initConflictVec();
		//buildWall(); //Uncomment if you want to have the door(s) "closed"
	}


private:

	double NEWexpFunction(int i, int j, int p);
	double probFunction(int i, int j, double maxFloorValues);
	double probFunctionCorrection(int i, int j, double maxFloorValues);

	void initMat();
	void initConflictVec();
	void statFieldInit();
	void groupMatrixFill();
	void startMat();
	void buildWall();

	void calcDL();
	void calcStatF();

	void calcProbVec(int p);
	void NEWcalcProbVec(int p);

	void isPedSafe(int k);
	void clearPed(int p);
	void pedDecideVect();
	void NEWPedDecide();

	void fillConflictVect();

	void findNResolveConflicts();

	void dynamicDecay();

	void resetDynField();
	void resetOccupied();
	void resetGroupMatrix();

	int coord2Indx(int p);

public:

	bool addPed(pedestrian & p1);
	void ranPed(int n);
	void densityPed(double density);
	void erasePed();

	void NEWdensityPed(double density, double paramDensity, double kD2, double kS2);

	void singleRun();
	void NEWtestRun();

	void printMovements();
	void printStatField();
	void printDynField();

	int numberOfPed();

	void writeMovements2File(std::string fileName);
	void writeStatField2File(std::string fileName);
	void writeDynField2File(std::string fileName);
	void writeData2File(std::string path);

	void NEWwriteMovements2File(std::string fileName);

	void changeSize(int _x, int _y);
	void changeKD(double _kD);
	void changeKS(double _kS);
	void printKs();
	double getKD();

	void changeAlpha(double _alpha);
	void changeDelta(double _delta);

	void resetFloor(int p);
	void resetFloor(double rho);

	floorPed(int x_, int y_, double kS_, double kD_, double alpha_, double beta_, std::vector<std::vector<int>> door_) {
		initializeFloor(x_, y_, kS_, kD_, alpha_, beta_, door_);
	}
};

