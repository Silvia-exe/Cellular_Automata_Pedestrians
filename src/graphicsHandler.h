#pragma once

#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>

#include"entity.h"
#include"essentialTools.h"

#define DISPLAY_BITMAP al_get_backbuffer(al_get_current_display())

using std::vector;
using essentialTools::point;

class graphicsHandler
{
	double squareSize;
	int width, height;

	vector<vector<entity>> rectangleGrid;

	ALLEGRO_BITMAP* screenBuffor;

	void initializeGrid();
	void initializeScreenBuffor();
	
public:

	graphicsHandler(int w, int h, double seperation)
	{
		width = w;
		height = h;
		squareSize = seperation;

		initializeGrid();
		initializeScreenBuffor();
	}

	bool insertEntityToGrid(entity toAdd, int x, int y)
	{
		if (y < rectangleGrid.size() && x < rectangleGrid.at(0).size())
		{
			rectangleGrid.at(y).at(x) = toAdd;
			return true;
		}
		return false;
	}

	void moveEntityOnTheGrid(point<int> from, point<int> to, bool swap);

	void drawTheGrid();
};