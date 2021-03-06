#include <iostream>
#include <ctime>
#include "pedestrian.h"
#include "floorPed.h"
#include "runs.h"

#include"graphicsHandler.h"

void anExampleOccupationVisualizationFunction(int occupiedValue, floorPed& peds, int roomWidth, int roomHeight)
{
	bool occupationStatus;
	for(int y = 0; y < roomHeight; y++)
	{
		for(int x = 0; x < roomWidth; x++)
		{
			occupationStatus = peds.getTheOccupationStatus(y, x);

			if (occupationStatus)
			{
				graphicsHandler::curHandler->setCellValue(x, y, occupiedValue);
			}
		}
	}

	graphicsHandler::curHandler->drawCells();
}

void addWalls(floorPed& peds, int wallBrightness, int wid, int hig)
{
	for(int y = 0; y < hig; y++)
	{
		for (int x = 0; x < wid; x++)
		{
			if (peds.getTheObstacleStatus(y, x) == false)
				graphicsHandler::curHandler->setCellValue(x, y, wallBrightness, true);
		}
	}
}

/*
 * wid: Display width
 * hig: Display height
 */
void initAllegro(int wid, int hig)
{
	//Inits allegro functions
	al_init();
	al_init_primitives_addon();

	//Creates the window to be drawn on
	ALLEGRO_DISPLAY* display = al_create_display(wid, hig);
}

int main() {

	initAllegro(1000,1000);

	srand(time(0));

	int n = 10; //Number of runs
	int x = 12; //x-dimension of room
	int y = 12; //y-dimension of room
	int p = 3; //Number of pedestrians in room

	graphicsHandler test(0, x, true, true);

	std::vector<std::vector<int>> door;
	door.push_back({ 0,5 });

	floorPed f1 = floorPed(x, y, 1, 0.5, 0.5, 0.1, door);
	f1.writeStatField2File("statFieldTest");
	f1.ranPed(p);

	f1.writeMovements2File("initialPosition");

	addWalls(f1, 200, 12, 12);

	for (int i = 0; i < n; i++) {

		f1.singleRunDynField();

		anExampleOccupationVisualizationFunction(100, f1, 12, 12);
		al_rest(0.1);
		
		f1.writeDynField2File("dynField_at" + std::to_string(i));
		f1.writeMovements2File("position_at" + std::to_string(i));
	}

	f1.writeMovements2File("finalPosition");

	runDiagConstPed(2, 10, 10, 10, door, "test");
}
