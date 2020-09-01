#include "stdafx.h"
#include <iostream>
#include <ctime>
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"


int main() {

	srand(time(0));

	int n = 10000; //Number of runs
	int x = 12; //x-dimension of room
	int y = 12; //y-dimension of room
	int p = 50; //Number of pedestrians in room
	double kD = 0;
	double kS = 1;
	double alpha = 0.5;
	double beta = 0.1;

	std::vector<std::vector<int>> door;
	door.push_back({ 0,5 });
	door.push_back({ 0,4 });

	floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, door);
	f1.ranPed(50);
	std::vector<std::string> dir = { "VarKDFolder" };
	std::string path = createDir(dir);

	f1.writeData2File(path);
	runDiagVarKD(f1, n, 0.95, "VarKDData", path);

}
