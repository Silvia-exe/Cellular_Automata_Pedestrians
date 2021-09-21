
#include "stdafx.h"
#include "runs.h"



/*Creates subfolders in the main project folder, subfolders will be created in the order they are in the vector
Returns the final path to the files for easy access*/
std::string createDir(std::vector<std::string> directories) {
	std::string hardDirec = "C:/Users/Silvia/source/repos/cellularAutomataNewVer/cellularAutomataNewVer/";
	std::string tempDirec = hardDirec;
	for (int i = 0; i < directories.size(); i++) {
		if (i != directories.size()) {
			tempDirec += directories[i] + "/";
		}
		else {
			tempDirec += directories[i];
		}
		_mkdir(tempDirec.c_str());
	}
	return tempDirec;
}


/*Runs n iterations of the singleRunDiag function with a floor with size x*y with p pedestrians. 
writeIterations should be 1 if you want the positions at every iteration to be written in a text file. 
Regardless, the final position of the pedestrians will be written in a text file with name "finalPosition"*/
void runDiagNRuns(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string path, bool writeIterations) {
	floorPed f1 = floorPed(x, y, 1, 0, 0, 0, doors);
	f1.ranPed(p);
	f1.writeStatField2File(path + "/staticField");
	f1.writeMovements2File(path + "/initialPosition");
	std::string itFolder = path + "/iterations";
	_mkdir(itFolder.c_str());

	if (writeIterations == 1) {
		for (int i = 0; i < n; i++) {
			f1.singleRunDiag();
			f1.writeMovements2File(itFolder + "/iteration" + std::to_string(i));
		}

		f1.writeMovements2File(path + "/finalPosition");
		
	}
	else {
		for (int i = 0; i < n; i++) {
			f1.singleRunDiag();
		}
		f1.writeMovements2File(path + "/finalPosition");
	}
}

void runDiagNRuns(floorPed f1, int n, std::string path, bool writeIterations){
	
	f1.writeStatField2File(path+"/staticField");
	f1.writeMovements2File(path+"/initialPosition");
	std::string itFolder = path + "/iterations";
	_mkdir(itFolder.c_str());

	if (writeIterations == 1) {
		for (int i = 0; i < n; i++) {
			f1.singleRunDynFieldMoore();
			f1.writeMovements2File(itFolder +"/iteration" + std::to_string(i));
			f1.writeDynField2File(path + "/dynField"+ std::to_string(i));
			
		}
		f1.writeDynField2File(path + "/dynFieldFinal");
		f1.writeMovements2File(path+"/finalPosition");
	}
	else {
		for (int i = 0; i < n; i++) {
			f1.singleRunDynFieldMoore();
		}
		f1.writeMovements2File(path+"/finalPosition");
	}
}


/*Runs iterations of the singleRunDynFieldMoore function with a floor with size x*y until all p pedestrians are saved. 
An upper limit n can be given to control the maximum number of iterations.
writeIterations should be 1 if you want the positions at every iteration to be written in a text file
Regardless, the final position of the pedestrians will be written in a text file with name "finalPosition"*/
void runDiagAllSavedMoore(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string path, bool writeIterations) {
	floorPed f1 = floorPed(x, y, 0, 0, 0, 0, doors);

	int it = 0;

	f1.writeStatField2File(path + "/staticField");
	f1.writeMovements2File(path + "/initialPosition");
	std::string itFolder = path + "/iterations";
	_mkdir(itFolder.c_str());


	if (writeIterations == 1) {
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDynFieldMoore();
			it++;
			f1.writeMovements2File(itFolder + "/iteration" + std::to_string(it));
		}
		f1.writeMovements2File(path + "/finalPosition");
	}
	else {
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDynFieldMoore();
			it++;
		}
		f1.writeMovements2File(path + "/finalPosition");
		std::cout << it << "\n";
	}
}

void runDiagAllSavedMoore(floorPed f1, int n,std::string path, bool writeIterations){
	
	int it = 0;

	f1.writeStatField2File(path + "/staticField");
	f1.writeMovements2File(path + "/initialPosition");
	std::string itFolder = path + "/iterations";
	_mkdir(itFolder.c_str());


	if (writeIterations == 1) {
		while (f1.numberOfSavedPed() != f1.numberOfPed() && it <= n) {
			f1.singleRunDynFieldMoore();
			it++;
			f1.writeMovements2File(itFolder + "/iteration" + std::to_string(it));
		}
		f1.writeMovements2File(path + "/finalPosition");
	}
	else {
		while (f1.numberOfSavedPed() != f1.numberOfPed() && it <= n) {
			f1.singleRunDynFieldMoore();
			it++;
		}
		f1.writeMovements2File(path + "/finalPosition");
		f1.writeDynField2File(path + "/dynamicField");
		std::cout << it << "\n";
	}
}

void runAllSavedVonNe(floorPed f1, int n, std::string path, bool writeIterations) {

	int it = 0;

	f1.writeStatField2File(path + "/staticField");
	f1.writeMovements2File(path + "/initialPosition");
	std::string itFolder = path + "/iterations";
	std::string dynFolder = path + "/dynField";
	_mkdir(itFolder.c_str());
	_mkdir(dynFolder.c_str());


	if (writeIterations == 1) {
		while (f1.numberOfSavedPed() != f1.numberOfPed() && it <= n) {
			f1.singleRunDynFieldVonNe();
			it++;
			f1.writeMovements2File(itFolder + "/iteration" + std::to_string(it));
			f1.writeDynField2File(dynFolder + "/dynIteration" + std::to_string(it));
		}
		f1.writeMovements2File(path + "/finalPosition");
	}
	else {
		while (f1.numberOfSavedPed() != f1.numberOfPed() && it <= n) {
			f1.singleRunDynFieldVonNe();
			it++;
		}
		f1.writeMovements2File(path + "/finalPosition");
		f1.writeDynField2File(path + "/dynamicField");
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

void runDiagConstPed(floorPed f1, int n, std::string fileName, std::string path) {
	srand(time(0));
	std::vector<int> dataIterations;
	std::ofstream file;
	int it = 0;
	for (int i = 0; i < n; i++) {
		it = 0;
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDiag();
			it++;
		}
		dataIterations.push_back(it);
	}

	file.open(path + "/" + fileName + ".txt");

	for (int i = 0; i < dataIterations.size(); i++) {
		file << i << "," << dataIterations[i] << "\n";
	}

	file.close();
}

/*Runs iterations of the singleRunDiag with a varying amount of pedestrians starting from 0 all the way to p until all pedestrians are saved
Or fills the room with varying density from 0 to rho and runs until all pedestrians are saved
This is done once
The total number of iterations for each run is noted down in a textFile with name fileName*/
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

void runDiagVarPed(floorPed f1, int p, std::string fileName, std::string path) {
	srand(time(0));
	std::vector<int> dataIterations;
	std::ofstream file;
	int it = 0;

	file.open(path + "/" + fileName + ".txt");

	file << "numbPed" << "iterations";
	for (int i = 0; i <= p; i++) {
		f1.resetFloor(i);
		it = 0;
		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			f1.singleRunDiag();
			it++;
		}
		dataIterations.push_back(it);
	}

	for (int i = 0; i < dataIterations.size(); i++) {
		file << i << ":" << dataIterations[i] << "\n";
	}

	file.close();
}

void runDiagVarPedRho(floorPed f1, double rho, std::string fileName, std::string path) {

	std::ofstream file;

	file.open(path + "/" + fileName + ".txt");
	
	int it = 0;
	int rhoInt = rho * 10;

	file << "density:iterations\n";

	for (double r = 0.5; r <= rhoInt; r +=0.5) {
		
		it = 0;
		f1.resetFloor(r);
		file << r << ":";

		while (f1.numberOfSavedPed() != f1.numberOfPed()) {
			
			f1.singleRunDiag();
			it++;
		}
		file << it << "\n";
	}

	file.close();
}

/*Runs iterations of singleRunDynFieldMoore varying the size from Xini and Yini to Xmax and Ymax until all pedestrians are saved
Size in x and y are noted now, afterwards the number of iterations that took for all pedestrians to escape is noted in a text file with name fileName*/
void runDiagVarSize(int Xmax, int Ymax, int Xini, int Yini, int p, double kD, double kS, double alpha, double beta, int maxIteration, std::string fileName, std::string path) {

	if (p > (Xini - 2)*(Yini - 2)) {
		std::cout << "Too many people" << "\n";
		exit;
	}

	int it = 0;
	std::ofstream file;
	std::vector<std::vector<int>> doors;
	
	file.open(path + "/" + fileName + ".txt");

	file << "x" << ":" << "y" << ":" << "iterations" << "\n";

	for (int x = Xini; x <= Xmax; x++) {
		for (int y = Yini; y <= Ymax; y++) {
			it = 0;
			doors.clear();
			doors.push_back({ 0, y / 2 });
			floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, doors);
			f1.ranPed(p);
			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIteration) {
				f1.singleRunDynFieldMoore();
				it++;
			}
			file << x << ":" << y << ":" << it << "\n";
		}
	}

	file.close();
}

void runDiagVarKD(floorPed f1, int n, double kD, std::string fileName, std::string path) {
	//std::cout << "Entered KDVar" << std::endl;
	int it = 0;
	int numPed = f1.numberOfPed();
	
	std::ofstream file;

	file.open(path + "/" +fileName + ".txt");
	file << "kD:iterations\n";

	for (double k = 0.0; k <= kD; k+=0.5) {
		f1.changeKD(k);
		f1.resetFloor(numPed);
		it = 0;

		file << k << ":";

		while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= n) {
			f1.singleRunDynFieldMoore();
			it++;
		}
		file << it << "\n";
	}
	file.close();
}

void runDiagVarAlpha(floorPed f1, int n, double alpha, std::string fileName, std::string path) {

	int it = 0;
	int alphaInt = alpha;
	int numbPed = f1.numberOfPed();

	std::ofstream file;
	file.open(path + "/" + fileName + ".txt");
	file << "alpha:iterations\n";
	
	for (double a = 0; a <= alphaInt; a += 0.5) {
		f1.changeAlpha(a);
		f1.resetFloor(numbPed);
		it = 0;

		file << a << ":";

		while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= n) {
			f1.singleRunDynFieldMoore();
			it++;
		}
		file << it << "\n";
	}
	file.close();
}

void runDiagVarBeta(floorPed f1, int n, double beta, std::string fileName, std::string path) {

	int it = 0;
	int betaInt = beta * 10;
	int numbPed = f1.numberOfPed();

	std::ofstream file;
	file.open(path + "/" + fileName + ".txt");
	file << "beta:iterations\n";

	for (double b = 0; b <= betaInt; b+=0.5) {
		f1.changeBeta(b);
		f1.resetFloor(numbPed);

		file << b << ":";

		while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= n) {
			f1.singleRunDynFieldMoore();
			it++;
		}
		file << it << "\n";
	}
	file.close();
}