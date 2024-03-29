#pragma once
#define EPSILON 0.00005

#include <ctime>
#include <string>
#include <direct.h>
#include "stdafx.h"
#include "floorPed.h"

std::string createDir(std::vector<std::string> directories);
void getDataKDStatic(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000);
void getDataKSStatic(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000);
void getDataChangePedDensityGroups(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int evacPed, int maxIt = 10000 );
int getRandomInt(int min, int max);
double getRandom01();
