#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include "mpi.h"
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"
#include <chrono>

using namespace std::chrono;


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

void getDataKSStatic(std::string path, floorPed f1, double kS, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKd_Ks" + std::to_string(kS) + "_" + std::to_string(i) + ".txt");

		for (double kD = 0.05; kD <= 10.0; kD += 0.05) {

			it = 0;
			f1.resetFloor(rho);
			f1.changeKD(kD);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIt) {
				f1.singleRunDynFieldVonNe();
				it++;
				
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
		}

		tempFile.close();

	}
};

void NEWgetDataKDStatic(std::string path, floorPed f1, double kD, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKs_Kd" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		for (double kS = 0.5; kS <= 10.0; kS += 0.5) {

			it = 0;
			f1.resetFloor(rho);
			f1.changeKS(kS);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIt) {
				f1.testRun();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
		}

		tempFile.close();
		std::cout << "Done with " << i << "." << std::endl;
	}
	
};

int main(int argc, char *argv[]) {

	//MPI_Init(&argc, &argv);

	//int rank, size;

	//MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(time(0));

	int n = 100000; //Number of runs
	int x = 65; //x-dimension of room
	int y = 65; //y-dimension of room
	int p = 2; //Number of pedestrians in room
	int m = 10; //Number of data files to be created

	double alpha = 0.1;
	double beta = 0.3;
	double kS = 10.0;
	double kD = 0.0;
	double rho = 0.3;

	std::vector<std::vector<int>> door;
	door.push_back({ 0, 31});

	auto start = high_resolution_clock::now();
	floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, door);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "Time initialize floorPed: " << duration.count() << std::endl;

	start = high_resolution_clock::now();
	f1.densityPed(rho);
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);

	// To get the value of duration use the count()
	// member function on the duration object
	std::cout << "Time densityPed: " << duration.count() << std::endl;

	start = high_resolution_clock::now();
	std::vector<std::string> dir = { "NewVersion/Data"};
	std::string path = createDir(dir);

	f1.writeData2File(path);
	f1.writeStatField2File(path + "staticField.txt");
	//std::cout << f1.numberOfPed() << " pedestrians initially" << std::endl;
	f1.writeMovements2File(path + "movement0");

	int initialPed = f1.numberOfPed();
	int i = 1;

	/*while (f1.numberOfSavedPed() != f1.numberOfPed()) {
		//std::cout << std::endl;
		//std::cout << "-------Started iteration " << i << "----------" << std::endl;
		f1.testRun();
		//f1.writeMovements2File(path + "movement" + std::to_string(i));
		i++;
		//std::cout << f1.numberOfPed() << std::endl;
	}*/

	//NEWgetDataKDStatic(path, f1, kD, rho, 0, 5);

	
	/*switch (rank)
	{
	case 0:
		std::cout << "Core " << rank << " working" << std::endl;
		NEWgetDataKDStatic(path, f1, kD, rho, 0, m / 4, n);
		break;
	case 1:
		std::cout << "Core " << rank << " working" << std::endl;
		NEWgetDataKDStatic(path, f1, kD, rho, m / 4, m / 2, n);
		break;
	case 2:
		std::cout << "Core " << rank << " working" << std::endl; 
		NEWgetDataKDStatic(path, f1, kD, rho, m / 2, 3 * m / 4, n);
		break;
	case 3:
		std::cout << "Core " << rank << " working" << std::endl;
		NEWgetDataKDStatic(path, f1, kD, rho, 3 * m / 4, m, n);
		break;
	}*/
	f1.testRun();

	//MPI_Finalize();
	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);

	// To get the value of duration use the count()
	// member function on the duration object
	std::cout << duration.count() << std::endl;

	

}