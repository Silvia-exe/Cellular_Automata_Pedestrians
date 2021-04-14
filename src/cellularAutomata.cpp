#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include "mpi.h"
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"


void getDataKDStatic(std::string path, floorPed f1, double kD, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKs_Kd" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		for (double kS = 0.05; kS <= 10.0; kS += 0.05) {

			it = 0;
			f1.resetFloor(rho);
			f1.changeKS(kS);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIt) {
				f1.singleRunDynFieldVonNe();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
		}

		tempFile.close();

	}
};


int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(time(0));

	int n = 100000; //Number of runs
	int x = 64; //x-dimension of room
	int y = 64; //y-dimension of room
	int p = 2; //Number of pedestrians in room
	int m = 100; //Number of data files to be created
	
	double alpha = 0.1;
	double beta = 0.3;
	double kS = 0;
	double kD = 0;
	double rho = 0.3;

	std::vector<std::vector<int>> door;
	door.push_back({0,31});
	
	floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, door);
	f1.densityPed(rho);

	std::vector<std::string> dir = { "VonNeumman", "MPI_test"};
	std::string path = createDir(dir);

	f1.writeData2File(path);

	switch (rank)
	{
	case 0:
		getDataKDStatic(path, f1, kD, rho, 0, m / 4, n);
		std::cout << "Core " << rank << " working" << std::endl;
		break;
	case 1:
		getDataKDStatic(path, f1, kD, rho, m/4, m /2, n);
		std::cout << "Core " << rank << " working" << std::endl;
		break;
	case 2 :
		getDataKDStatic(path, f1, kD, rho, m/2, 3*m/4, n);
		std::cout << "Core " << rank << " working" << std::endl;
		break;
	case 3:
		getDataKDStatic(path, f1, kD, rho, 3*m/4, m, n);
		std::cout << "Core " << rank << " working" << std::endl;
		break;
	}
	
	
	MPI_Finalize();

}
