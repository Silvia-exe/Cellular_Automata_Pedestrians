#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#include "mpi.h"
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"


using namespace std::chrono;

void getDataKDStatic(std::string path, floorPed f1, double kD, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKs_Kd" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		/*if (i == nDataFilesI)
		{
			std::cout << "Entered clean run" << std::endl;
			f1.testRun();
			std::cout << "Done with cleaning run" << std::endl;
		}*/

		for (double kS = 0.1; kS <= 4.0; kS += 0.05) {

			it = 0;
			f1.resetFloor(rho);
			f1.changeKS(kS);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIt) {
				//f1.singleRunDynFieldVonNe();
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

void getDataKSStatic(std::string path, floorPed f1, double kS, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000) {

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKd_Ks" + std::to_string(kS) + "_" + std::to_string(i) + ".txt");

		for (double kD = 4.0; kD <= 10.0; kD += 0.5) {

			it = 0;
			f1.resetFloor(rho);
			f1.changeKD(kD);

			while (f1.numberOfPed() != f1.numberOfSavedPed() && it <= maxIt) {
				//f1.singleRunDynFieldVonNe();
				f1.testRun();
				it++;
			}
			tempFile << kD << ":" << kS << ":" << it << "\n";
			std::cout << "Done with " << kD << "." << std::endl;
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

int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(time(0));
	std::vector<std::vector<int>> door;
	std::vector<std::string> dir = { "/FinalVersion" };
	std::string path = createDir(dir);

	/*Floor parameters and data files parameters*/
	int n = 100000; //Number of runs
	int x = 63; //x-dimension of room
	int y = 63; //y-dimension of room
	int p = 2; //Number of pedestrians in room
	int m = 50; //Number of data files to be created

	double alpha = 0.1; //Probability of difussion
	double beta = 0.3; //Probability of decay
	double rho = 0.3; //Density of total pedestrians added to the field
	door.push_back({0, 31});

	/*Group 1 parameters*/
	double parameterRho = 0.9; //Number of pedestrians to be in parameter group 1
	double kS_secnd = 1.0; //Second set of sensitivity parameters
	double kD_secnd = 10.0;

	/*Group 2 parameters*/
	double kS = 10.0;
	double kD = 10.0;

	/*Initializing floor and filling it with pedestrians*/
	floorPed f1 = floorPed(x, y, kS, kD, alpha, beta, door);
	floorPed f2 = floorPed(x, y, kS, kD, alpha, beta, door);
	floorPed f3 = floorPed(x, y, kS, kD, alpha, beta, door);
	floorPed f4 = floorPed(x, y, kS, kD, alpha, beta, door);
	//f1.NEWdensityPed(rho,parameterRho,kD_secnd,kS_secnd);
	f1.densityPed(rho);
	//pedestrian p1 = pedestrian(4, 4);
	//pedestrian p2 = pedestrian(4, 6);
	//f1.addPed(p1);
	//f1.addPed(p2);
	f2.densityPed(rho);
	f3.densityPed(rho);
	f4.densityPed(rho);

	f1.writeData2File(path);
	f1.writeStatField2File(path + "staticField.txt");
	f1.writeMovements2File(path + "movement0");

	int initialPed = f1.numberOfPed();
	int i = 1;

	/*std::cout << "Test Run 1." << std::endl;
	f1.testRun();
	std::cout << "Test Run 2." << std::endl;
	f1.testRun();
	std::cout << "Test Run 3." << std::endl;
	f1.testRun();
	std::cout << "End of tests" << std::endl;*/

	/*while (f1.numberOfPed() != 0) {
		f1.NEWtestRun();
		f1.NEWwriteMovements2File(path + "movement" + std::to_string(i));
		//std::cout << f1.numberOfPed() << std::endl;
		i++;
	}*/

	/*while (f1.numberOfSavedPed() != f1.numberOfPed()) {
		//std::cout << std::endl;
		std::cout << "----------Started iteration " << i << "----------" << std::endl;
		f1.testRun();
		//f1.writeMovements2File(path + "movement" + std::to_string(i));
		//f1.writeDynField2File(path + "dynField" + std::to_string(i));
		i++;
		//std::cout << f1.numberOfPed() << std::endl;
	}*/

	//getDataKDStatic(path, f1, kD, rho, 0, 1);

	switch (rank)
	{
	case 0:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKDStatic(path, f1, kD, rho, 0, m / 4, n);
		break;
	case 1:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKDStatic(path, f2, kD, rho, m / 4, m / 2, n);
		break;
	case 2:
		std::cout << "Core " << rank << " working" << std::endl; 
		getDataKDStatic(path, f3, kD, rho, m / 2, 3 * m / 4, n);
		break;
	case 3:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKDStatic(path, f4, kD, rho, 3 * m / 4, m, n);
		break;
	}

	MPI_Finalize();

}