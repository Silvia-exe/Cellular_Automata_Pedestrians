#include "stdafx.h"
#include <iostream>
#include <ctime>
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"


int main() {

	srand(time(0));

	int n = 100000; //Number of runs
	int x = 64; //x-dimension of room
	int y = 64; //y-dimension of room
	int p = 50; //Number of pedestrians in room
	
	double alpha = 0.5;
	double beta = 0.5;

	std::vector<std::vector<int>> door;
	door.push_back({ 0, 32 });

	floorPed f1 = floorPed(x, y, 0, 0, alpha, beta, door);
	f1.densityPed(0.25);

	std::vector<std::string> dir = { "VarKD_rho25" };
	std::string path = createDir(dir);
	f1.writeData2File(path);

	for (double kS = 0.5; kS <= 10.0; kS += 0.5) {
		f1.changeKS(kS);
		runDiagVarKD(f1, n, 10.0, "VarKD_KS" + std::to_string(kS), path);
		f1.writeDynField2File(path + "/DynField" + std::to_string(kS));
		std::cout << "Done with " << kS;
	}
}
