#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"


int main() {

	srand(time(0));

	int n = 10000; //Number of runs
	int x = 43; //x-dimension of room
	int y = 43; //y-dimension of room
	int p = 50; //Number of pedestrians in room
	
	double alpha = 0.5;
	double beta = 0.5;
	double kS = 1.0;
	double kD = 2.0;

	std::vector<std::vector<int>> door;
	door.push_back({0,22});

	floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, door);
	f1.densityPed(0.3);

	std::vector<std::string> dir = { "divulgationGraphs", "openDoor"};
	std::string path = createDir(dir);

	f1.writeData2File(path);

	runDiagNRuns(f1, n, path, 1);

	/*for (int i = 0; i < 100; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKs_Kd" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		for (double kD = 0.05; kD <= 10.0; kD += 0.05) {

			it = 0;
			f1.resetFloor(0.3);
			f1.changeKD(kD);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= n) {
				f1.singleRunDynField();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
			//std::cout << "It works " << kD << " " << kS << "\n";
		}

		tempFile.close();

	}
	
	/*
	for (int i = 0; i <= 15; i++) {
		for (double j = 0.5; j <= 10.0; j += 0.5) {
			storeIter.clear();
			mean.open("varKD_KS_" + std::to_string(i) + "/varKd_Ks" + std::to_string(j) + "_" + std::to_string(i) + ".txt");
			while (std::getline(mean, temp, ':')) {
				storeIter.push_back(temp);
			}
		}
		mean.close();
	}

	for (int i = 0; i < meanIter.size(); i++) {
		std::cout << meanIter[i] << "\n";
	}*/

}
