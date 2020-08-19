#pragma once

#include <ctime>
#include "stdafx.h"
#include "floorPed.h"


void runDiagNRuns(int p, int n, int x, int y, std::vector<std::vector<int>> doors, bool writeIterations);
void runDiagAllSaved(int p, int n, int x, int y, std::vector<std::vector<int>> doors, bool writeIterations);
void runDiagConstPed(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string fileName);
void runDiagVarPed(int p, int x, int y, std::vector<std::vector<int>> doors, std::string fileName);
void runDiagVarSizeDynField(int Xmax, int Ymax, int Xini, int Yini, int p, double kD, double kS, double alpha, double beta, int maxIteration, std::string fileName);