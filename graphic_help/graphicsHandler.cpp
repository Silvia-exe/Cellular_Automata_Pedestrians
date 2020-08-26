#include "graphicsHandler.h"

graphicsHandler* graphicsHandler::curHandler = nullptr;

void graphicsHandler::initializeCells(int n, int defaultValue)
{	
	vector<int> temp;
	vector<bool> tempPerm;
	for (int y = 0; y < n; y++)
	{
		temp.clear();

		for(int x = 0; x < n; x++)
		{
			temp.emplace_back(defaultValue);
			tempPerm.emplace_back(false);
		}

		cellStatus.emplace_back(temp);
		isPermament.emplace_back(tempPerm);
	}

	//Adjusts the cell dimensions in accordance to created window
	cellWidth = al_get_display_width(al_get_current_display()) / n;
	cellHeight = al_get_display_height(al_get_current_display()) / n;
}

void graphicsHandler::resetBoard()
{
	for (int y = 0; y < cellStatus.size(); y++)
	{
		for (int x = 0; x < cellStatus.size(); x++)
		{
			if (isPermament.at(y).at(x) == false)
				cellStatus.at(y).at(x) = zeroValue;
		}
	}
}

void graphicsHandler::drawCells(bool clearScreen, bool flipScreen)
{
	if(clearScreen)
		al_clear_to_color(al_map_rgb(0, 0, 0));

	const int lowestYValue = cellStatus.size() * cellWidth;
	int curCellStatus = 0;

	for (int y = 0; y < cellStatus.size(); y++)
	{
		for (int x = 0; x < cellStatus.size(); x++)
		{
			curCellStatus = cellStatus.at(y).at(x);
			al_draw_filled_rectangle(x * cellWidth, lowestYValue - y * cellHeight, (x + 1) * cellWidth, lowestYValue - (y + 1) * cellHeight, al_map_rgb(curCellStatus, curCellStatus, curCellStatus));
		}
	}

	if (isBoardResetEachDraw)
		resetBoard();

	if (flipScreen)
		al_flip_display();
}