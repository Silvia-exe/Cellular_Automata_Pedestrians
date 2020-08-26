#include<vector>

#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

using std::vector;

class graphicsHandler
{
	vector<vector<int>> cellStatus;
	vector<vector<bool>> isPermament; //If set to true, the cell under the coordinate wont ever be automatically cleared
	int zeroValue;

	int cellWidth;
	int cellHeight;

	bool isBoardResetEachDraw;

	void initializeCells(int n, int defaultValue);
	void resetBoard();

public:

	static graphicsHandler* curHandler;

	/*
	 * defaultValue: Default value for a cell, corresponds to the RGB color values where
	 * R = G = B = defaultValue
	 * 
	 * amountOfCells: Amount of cells on each axis, where total amount of cells is
	 * amountOfCells*amountOfCells
	 *
	 * resetBoard: If set to true, each time the draw function is called, all non-permanent
	 * cells have their value reset to zeroValue(defaultValue)
	 *
	 * setToBeACurrentHandler: Assigns this instance of a class to the static pointer
	 * making it accessible from anywhere in the program using graphicsHandler::curHandler 
	 */
	graphicsHandler(int defaultValue, int amountOfCells, bool resetBoard, bool setToBeACurrentHandler)
	{
		zeroValue = defaultValue;

		initializeCells(amountOfCells, defaultValue);

		isBoardResetEachDraw = resetBoard;

		if(setToBeACurrentHandler)
			curHandler = this;
	}

	void setCellValue(int x, int y, int value, bool permament = false)
	{
		cellStatus.at(y).at(x) = value;
		isPermament.at(y).at(x) = permament;
	}

	/*
	 * clearScreen: If set to true, will clear the screen
	 * flipScreen: Internal allegro function, flips the buffor with the newly drawn on
	 */
	void drawCells(bool clearScreen = true, bool flipScreen = true);
};