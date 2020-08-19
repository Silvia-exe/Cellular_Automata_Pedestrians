
#include "stdafx.h"
#include "runs.h"

/*Runs n iterations of the singleRunDiag function with a floor with size x*y with p pedestrians. 
writeIterations should be 1 if you want the positions at every iteration to be written in a text file. 
Regardless, the final position of the pedestrians will be written in a text file with name "finalPosition"*/
void runDiagNRuns(int p, int n, int x, int y, std::vector<std::vector<int>> doors, bool writeIterations) {
	floorPed f1 = floorPed(x, y, 1, 0, 0, 0, doors);
	f1.ranPed(p);
	f1.writeStatField2File("staticField");
	f1.writeMovements2File("initialPosition");

	if (writeIterations == 1) {
		for (int i = 0; i < n; i++) {
			f1.singleRunDiag();
			f1.writeMovements2File("iteration" + std::to_string(i));
		}
		f1.writeMovements2File("finalPosition");
	}
	else {
		for (int i = 0; i < n; i++) {
			f1.singleRunDiag();
		}
		f1.writeMovements2File("finalPosition");
	}
}


/*Runs iterations of the singleRunDiag function with a floor with size x*y until all p pedestrians are saved. 
An upper limit n can be given to control the maximum number of iterations.
writeIterations should be 1 if you want the positions at every iteration to be written in a text file
Regardless, the final position of the pedestrians will be written in a text file with name "finalPosition"*/
void runDiagAllSaved(int p, int n, int x, int y, std::vector<std::vector<int>> doors, bool writeIterations) {
	floorPed f1 = floorPed(x, y, 1, 0, 0.7, 0.8, doors);
	f1.ranPed(p);
	f1.writeStatField2File("staticField");
	f1.writeMovements2File("initialPosition");
	int it = 0;

	if (writeIterations == 1) {
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDiag();
			it++;
			f1.writeMovements2File("iteration" + std::to_string(it));
		}
		f1.writeMovements2File("finalPosition");
	}
	else {
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDiag();
			it++;
		}
		f1.writeMovements2File("finalPosition");
		std::cout << it << "\n";
	}
}


/*Runs iterations of the singleRunDiag function with a floor with size x*y and p pedestrians until all pedestrians are saved.
This is done n amount of times.
The total number of runs until all pedestrians are saved is written in a text file with name fileName*/
void runDiagConstPed(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string fileName) {
	srand(time(0));
	std::vector<int> dataIterations;
	std::ofstream file;
	int it = 0;
	for (int i = 0; i < n; i++) {
		it = 0;
		floorPed f1 = floorPed(x, y, 1, 0, 0.7, 0.8, doors);
		f1.ranPed(p);
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDiag();
			it++;
		}
		dataIterations.push_back(it);
	}

	file.open(fileName + ".txt");

	for (int i = 0; i < dataIterations.size(); i++) {
		file << i << "," << dataIterations[i] << "\n";
	}

	file.close();
}

void runDiagVarPed(int p, int x, int y, std::vector<std::vector<int>> doors, std::string fileName) {
	srand(time(0));
	std::vector<int> dataIterations;
	std::ofstream file;
	int it = 0;
	for (int i = 0; i < p; i++) {
		it = 0;
		floorPed f1 = floorPed(x, y, 1, 0, 0.7, 0.8, doors);
		f1.ranPed(i);
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDiag();
			it++;
		}
		dataIterations.push_back(it);
	}

	file.open(fileName + ".txt");

	for (int i = 0; i < dataIterations.size(); i++) {
		file << i << "," << dataIterations[i] << "\n";
	}

	file.close();
}

void runDiagVarSizeDynField(int Xmax, int Ymax, int Xini, int Yini, int p, double kD, double kS, double alpha, double beta, int maxIteration, std::string fileName) {

	if (p > (Xini - 2)*(Yini - 2)) {
		std::cout << "Too many people" << "\n";
		exit;
	}

	int it = 0;
	std::ofstream file;
	file.open(fileName + ".txt");

	file << "x" << ":" << "y" << ":" << "iterations" << "\n";

	for (int x = Xini; x <= Xmax; x++) {
		for (int y = Yini; y <= Ymax; y++) {
			it = 0;
			std::vector<std::vector<int>> doors;
			std::vector<int> d1 = { 0, y / 2 };
			doors.push_back(d1);
			floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, doors);
			f1.ranPed(p);
			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIteration) {
				f1.singleRunDynField();
				it++;
			}
			file << x << ":" << y << ":" << it << "\n";
		}
	}

	file.close();
}