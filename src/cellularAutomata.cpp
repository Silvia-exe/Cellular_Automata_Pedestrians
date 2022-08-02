#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#include "mpi.h"
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"
	
//#define USE_MPI
using namespace std::chrono;

void getDataKDStatic(std::string path, floorPed* f1, double kD, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKs_Kd" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		for (double kS = 0.05; kS <= 10.0; kS += 0.05) {

			it = 0;
			
			f1->changeKS(kS);

			while (f1->numberOfPed() != 0 && it <= maxIt) {
				//f1.singleRunDynFieldVonNe();
				f1->testRun();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
			it = 0;
			std::cout << "Done with " << kS << "." << std::endl;
			f1->resetFloor(rho);
		}

		tempFile.close();
		std::cout << "Done with " << i << "." << std::endl;
	}
};

void getDataKSStatic(std::string path, floorPed* f1, double kS, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKd_Ks" + std::to_string(kS) + "_" + std::to_string(i) + ".txt");

		for (double kD = 0.1; kD <= 5.0; kD += 0.1) {

			it = 0;
			f1->changeKD(kD);

			while (f1->numberOfPed() != 0 && it <= maxIt) {
				//f1.singleRunDynFieldVonNe();
				f1->testRun();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
			std::cout << "Done with " << kD << "." << std::endl;

			f1->resetFloor(rho);
		}
		tempFile.close();
	}
};

void NEWgetDataKDStatic(std::string path, floorPed f1, double kD, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/VarKs_StatickD" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		for (double kS = 0.5; kS <= 10.0; kS += 0.5) {

			it = 0;
			f1.resetFloor(rho);
			f1.changeKS(kS);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIt) {
				f1.testRun();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
			std::cout << "Done with " << kS << "." << std::endl;
		}

		tempFile.close();
		std::cout << "Done with " << i << "." << std::endl;
	}
	
};

int main(int argc, char* argv[]) {

#ifdef USE_MPI
	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif

	srand(time(0));
	std::vector<std::vector<int>> door;
	std::vector<std::string> dirMov = { "/FinalVersion/Movements" };
	std::vector<std::string> dirDF = { "/FinalVersion/DF" };
	std::vector<std::string> dir = { "/FinalVersion" };
	std::string pathMov = createDir(dirMov);
	std::string pathDF = createDir(dirDF);
	std::string path = createDir(dir);

	/*Floor parameters and data files parameters*/
	int n = 10000; //Number of runs
	int x = 63; //x-dimension of room
	int y = 63; //y-dimension of room
	int p = 2; //Number of pedestrians in room
	int m = 4; //Number of data files to be created

	double alpha = 0.3; //Probability of difussion
	double beta = 0.3; //Probability of decay
	double rho = 0.3; //Density of total pedestrians added to the field
	door.push_back({ 0, 31 });

	/*Group 1 parameters*/
	double parameterRho = 0.9; //Number of pedestrians to be in parameter group 1
	double kS_secnd = 0.1; //Second set of sensitivity parameters
	double kD_secnd = 10.0;

	/*Group 2 parameters*/
	double kS = 0.4;
	double kD = 2.3;

	/*Initializing floor and filling it with pedestrians*/
	floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, door);
	floorPed f2 = floorPed(x, y, kS, kD, alpha, beta, door);
	floorPed f3 = floorPed(x, y, kS, kD, alpha, beta, door);
	floorPed f4 = floorPed(x, y, kS, kD, alpha, beta, door);

	f1.densityPed(rho);
	f2.densityPed(rho);
	f3.densityPed(rho);
	f4.densityPed(rho);

	f1.writeData2File(path);
	f1.writeStatField2File(path + "staticField.txt");
	f1.writeMovements2File(pathMov + "movement0");

	int initialPed = f1.numberOfPed();
	int i = 0;

	while (f1.numberOfPed() != 0) {
		//std::cout << std::endl;
		//std::cout << "----------Started iteration " << i << "----------" << std::endl;
		f1.testRun();
		f1.writeMovements2File(pathMov + "movement" + std::to_string(i));
		f1.writeDynField2File(pathDF + "dynField" + std::to_string(i));
		i++;
		//std::cout << f1.numberOfPed() << std::endl;
	}
	std::cout << "# of its: " << i << std::endl;

	//getDataKSStatic(path, &f1, kS, rho, 0, 1, n);

	#ifdef USE_MPI
	switch (rank)
	{
	case 0:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path, &f1, kS, rho, 0, m / 4, n);
		break;
	case 1:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path, &f2, kS, rho, m / 4, m / 2, n);
		break;
	case 2:
		std::cout << "Core " << rank << " working" << std::endl; 
		getDataKSStatic(path, &f3, kS, rho, m / 2, 3 * m / 4, n);
		break;
	case 3:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path, &f4, kS, rho, 3 * m / 4, m, n);
		break;
	}

	
		MPI_Finalize();
	#endif

}