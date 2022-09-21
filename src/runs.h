#pragma once

#include <ctime>
#include <string>
#include <direct.h>
#include "stdafx.h"
#include "floorPed.h"

std::string createDir(std::vector<std::string> directories);
void getDataKDStatic(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000);
void getDataKSStatic(std::string path, floorPed* f1, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000);
void NEWgetDataKDStatic(std::string path, floorPed* f1, double kD, double rho, int nDataFilesI, int nDataFilesF, int maxIt = 10000);
int getRandomInt(int min, int max);
double getRandom01();
double randomNumber01(int decimalSpaces);