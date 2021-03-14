#pragma once

#include <ctime>
#include <string>
#include <direct.h>
#include "stdafx.h"
#include "floorPed.h"

std::string createDir(std::vector<std::string> directories);

void runDiagNRuns(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string path, bool writeIterations);
void runDiagNRuns(floorPed f1, int n, std::string path, bool writeIterations);
void runDiagNRuns(floorPed f1, int n, std::string path, bool writeIterations);

void runDiagAllSavedMoore(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string path, bool writeIterations);
void runDiagAllSavedMoore(floorPed f1, int n, std::string path, bool writeIterations);
void runAllSavedVonNe(floorPed f1, int n, std::string path, bool writeIterations);

void runDiagConstPed(int p, int n, int x, int y, std::vector<std::vector<int>> doors, std::string fileName);
void runDiagConstPed(floorPed f1, int n, std::string fileName, std::string path);

void runDiagVarPed(int p, int x, int y, std::vector<std::vector<int>> doors, std::string fileName);
void runDiagVarPed(floorPed f1, int p, std::string fileName, std::string path);
void runDiagVarPedRho(floorPed f1, double rho, std::string fileName, std::string path);

void runDiagVarSize(int Xmax, int Ymax, int Xini, int Yini, int p, double kD, double kS, double alpha, double beta, int maxIteration, std::string fileName, std::string path);

void runDiagVarKD(floorPed f1, int n, double kD, std::string fileName, std::string path);

void runDiagVarAlpha(floorPed f1, int n, double alpha, std::string fileName, std::string path);
void runDiagVarBeta(floorPed f1, int n, double beta, std::string fileName, std::string path);