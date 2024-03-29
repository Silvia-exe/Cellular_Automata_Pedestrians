#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#include "mpi.h"
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"

//#define USE_MPI //Comment if wont use multithreading
using namespace std::chrono;

int main(int argc, char* argv[]) {

#ifdef USE_MPI
	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
#endif
	
	std::vector<std::vector<int>> door; //Initializes door vector. Will hold door coordinates in pairs.
	std::vector<std::string> dirMov = { "/FinalVersion/Movements" }; //Directory which will hold movements (if saved)
	std::vector<std::string> dirDF = { "/FinalVersion/DF" }; //Directory which will hold dynamic fields (if saved)
	std::vector<std::string> dir04 = { "/FinalVersion/groupPedTest"}; //Directory that will hold data files
	std::vector<std::string> dir1 = { "/FinalVersion/ks1" }; //Directory that will hold data files
	std::vector<std::string> dir10 = { "/FinalVersion/ks10" }; //Directory that will hold data files
	std::string pathMov = createDir(dirMov); //Creates directories with specified addresses
	std::string pathDF = createDir(dirDF);
	std::string path04 = createDir(dir04);
	//std::string path1 = createDir(hardDir, dir1);
	//std::string path10 = createDir(hardDir, dir10);

	/*Floor parameters and data files parameters*/
	int n = 50000; //Number of runs
	int x = 63; //x-dimension of room
	int y = 63; //y-dimension of room
	int p = 2; //Number of pedestrians in room
	int m = 40; //Number of data files to be created

	double alpha = 0.0; //Probability of difussion
	double delta = 0.4; //Probability of decay
	double rho = 0.3; //Density of total pedestrians added to the field
	double completeEvacuation = 0.95;
	door.push_back({ 0, 31 }); //Door cordinates, given in pairs.

	/*Group 1 parameters, ignore for now*/
	double parameterRho = 0.5; //Number of pedestrians to be in parameter group 1
	double kS_secnd = 10.0; //Second set of sensitivity parameters
	double kD_secnd = 0.0;

	/*Group 2 parameters*/
	double kS = 0.4; //Static floor weighting parameter
	double kD = 1.0; //Dynamic floor weighting parameter

	/*Initializing floor and filling it with pedestrians*/
	floorPed f0 = floorPed(x, y, kS, kD, alpha, delta, door, kS_secnd, kD_secnd);
	floorPed f1 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f2 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f3 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f4 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f5 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f6 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f7 = floorPed(x, y, kS, kD, alpha, delta, door);

	//f0.densityPed(rho);
	//f1.densityPed(rho);
	f2.densityPed(rho);
	f3.densityPed(rho);
	f4.densityPed(rho);
	f5.densityPed(rho);
	f6.densityPed(rho);
	f7.densityPed(rho);

	f0.densityPedGroups(rho, parameterRho);

	f1.writeData2File(path04);
	f1.writeStatField2File(path04 + "staticField");
	//f1.writeMovements2File(pathMov + "movement0");

	int initPed = f1.numberOfPed();
	int evacPed = initPed * completeEvacuation;
	int i = 0;

	/*while (f1.getSavedPed() < evacPed) {
		//std::cout << std::endl;
		std::cout << "----------Started iteration " << i << "----------" << std::endl;
		f1.singleRun();
		std::cout << "Number of Ped: " << f1.numberOfPed() << std::endl;
		f1.writeDynField2File(pathDF + "dynField" + std::to_string(i));
		//f1.writeGroupMovements2File(pathMov + "groupMovement" + std::to_string(i));
		i++;
		//std::cout << f1.numberOfPed() << std::endl;
	}*/
	//std::cout << "# of its: " << i << std::endl;

	getDataChangePedDensityGroups(path04, &f0, rho, 0, 2, evacPed, n);

#ifdef USE_MPI
	switch (rank)
	{
	case 0:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f0, rho, 0, m / 8, n);
		break;
	case 1:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f1, rho, m / 8, m / 4, n);
		break;
	case 2:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f2, rho, m / 4, 3 * m / 8, n);
		break;
	case 3:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f3, rho, 3 * m / 8, m / 2, n);
		break;
	case 4:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f4, rho, m / 2, 5 * m / 8, n);
		break;
	case 5:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f5, rho, 5 * m / 8, 3 * m / 4, n);
		break;
	case 6:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f6, rho, 3 * m / 4, 7 * m / 8, n);
		break;
	case 7:
		std::cout << "Core " << rank << " working" << std::endl;
		getDataKSStatic(path04, &f7, rho, 7 * m / 8, m, n);
		break;
	}

	MPI_Finalize();
#endif

}