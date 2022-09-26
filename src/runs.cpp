
#include "stdafx.h"
#include "runs.h"

/*Creates subfolders in the main project folder, subfolders will be created in the order they are in the vector
Returns the final path to the files for easy access*/
std::string createDir(std::vector<std::string> directories) {
	std::string hardDirec = "C:/Users/DellPC/source/repos/CellularAutomataPedestrians/";
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

void getDataKDStatic(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int maxIt) {

	double kD = f1->getKD();

	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKs_Kd" + std::to_string(kD) + "_" + std::to_string(i) + ".txt");

		for (double kS = 0.1; kS <= 4.0; kS += 0.1) {

			it = 0;

			f1->changeKS(kS);

			while (f1->numberOfPed() != 0 && it <= maxIt) {
				f1->singleRun();
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

void getDataKSStatic(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int maxIt) {

	double kS = f1->getKS();
	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/varKd_Ks" + std::to_string(kS) + "_" + std::to_string(i) + ".txt");

		for (double kD = 0.0; kD <= 10.0; kD += 0.1) {

			it = 0;
			f1->changeKD(kD);
			std::cout << "Running for " << kD << std::endl;

			while (f1->numberOfPed() != 0 && it <= maxIt) {
				f1->singleRun();
				it++;
			}

			tempFile << kD << ":" << kS << ":" << it << "\n";
			it = 0;
			std::cout << "Done with " << kD << "." << std::endl;

			f1->resetFloor(rho);
		}
		tempFile.close();
	}
};

void getDataChangePedDensityGroups(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int evacPed, int maxIt) {


	for (int i = nDataFilesI; i < nDataFilesF; i++) {

		int it = 0;

		std::ofstream tempFile;
		tempFile.open(path + "/VarRho_StaticPar" + std::to_string(i) + ".txt");

		for (double groupRho = 0.05; groupRho <= 1.0; groupRho += 0.05) {

			it = 0;
			f1->resetFloorGroups(rho, groupRho);

			while (f1->numberOfPed() != 0 && it <= maxIt) {
				f1->singleRun();
				it++;
			}
			tempFile << groupRho << ":" << it << "\n";
			std::cout << "Done with " << groupRho << "." << std::endl;
		}

		tempFile.close();
		std::cout << "Done with " << i << "." << std::endl;
	}

};