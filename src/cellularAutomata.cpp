#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#include "mpi.h"
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"
	
#define USE_MPI //Comment if wont use multithreading
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
	std::vector<std::string> dir = { "/FinalVersion" }; //Directory that will hold data files
	std::string pathMov = createDir(dirMov); //Creates directories with specified addresses
	std::string pathDF = createDir(dirDF);
	std::string path = createDir(dir);

	/*Floor parameters and data files parameters*/
	int n = 50000; //Number of runs
	int x = 63; //x-dimension of room
	int y = 63; //y-dimension of room
	int p = 2; //Number of pedestrians in room
	int m = 25; //Number of data files to be created

	double alpha = 0.3; //Probability of difussion
	double delta = 0.3; //Probability of decay
	double rho = 0.3; //Density of total pedestrians added to the field
	door.push_back({ 0, 31 }); //Door cordinates, given in pairs.

	/*Group 1 parameters, ignore for now*/
	double parameterRho = 0.9; //Number of pedestrians to be in parameter group 1
	double kS_secnd = 0.1; //Second set of sensitivity parameters
	double kD_secnd = 10.0;

	/*Group 2 parameters*/
	double kS = 1.0; //Static floor weighting parameter
	double kD = 10.0; //Dynamic floor weighting parameter

	/*Initializing floor and filling it with pedestrians*/
	floorPed f1 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f2 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f3 = floorPed(x, y, kS, kD, alpha, delta, door);
	floorPed f4 = floorPed(x, y, kS, kD, alpha, delta, door);

	f1.densityPed(rho);
	f2.densityPed(rho);
	f3.densityPed(rho);
	f4.densityPed(rho);

	f1.writeData2File(path);
	f1.writeStatField2File(path + "staticField.txt");
	f1.writeMovements2File(pathMov + "movement0");

	int initialPed = f1.numberOfPed();
	int i = 0;

	/*while (f1.numberOfPed() != 0) {
		//std::cout << std::endl;
		std::cout << "----------Started iteration " << i << "----------" << std::endl;
		f1.testRun();
		//f1.writeMovements2File(pathMov + "movement" + std::to_string(i));
		//f1.writeDynField2File(pathDF + "dynField" + std::to_string(i));
		i++;
		//std::cout << f1.numberOfPed() << std::endl;
	}*/
	//std::cout << "# of its: " << i << std::endl;

	//getDataKSStatic(path, &f1, kS, rho, 0, 5, n);

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