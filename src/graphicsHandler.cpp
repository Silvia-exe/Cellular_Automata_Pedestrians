#include "graphicsHandler.h"

void graphicsHandler::initializeGrid()
{
	vector<entity> temp;

	for (int y = 0; y < height; y++)
	{
		temp.clear();
		for (int x = 0; x < width; x++)
		{
			temp.emplace_back(entity());
		}
		rectangleGrid.emplace_back(temp);
	}
}

void graphicsHandler::initializeScreenBuffor()
{
	screenBuffor = al_create_bitmap(width * squareSize, height * squareSize);
	
	ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
	al_set_target_bitmap(screenBuffor);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_set_target_bitmap(prev_target);
}

void graphicsHandler::moveEntityOnTheGrid(point<int> from, point<int> to, bool swap)
{
	entity oldValue;

	if (swap)
		oldValue = rectangleGrid.at(to.y).at(to.x);

	rectangleGrid.at(to.y).at(to.x) = rectangleGrid.at(from.y).at(from.x);

	if (swap)
		rectangleGrid.at(from.y).at(from.x) = oldValue;
	else
		rectangleGrid.at(from.y).at(from.x) = entity();
}

void graphicsHandler::drawTheGrid()
{
	al_set_target_bitmap(screenBuffor);
	
	al_lock_bitmap(screenBuffor, al_get_bitmap_format(screenBuffor), ALLEGRO_LOCK_WRITEONLY);
	for (int y = 0; y < rectangleGrid.size() - 1; y++)
	{
 		for (int x = 0; x < rectangleGrid[y].size() - 1; x++)
		{
			if (rectangleGrid.at(y).at(x).drawable)
				al_draw_filled_rectangle(x * squareSize, y * squareSize, (x + 1) * squareSize, (y + 1) * squareSize, rectangleGrid.at(y).at(x).col);
			if (rectangleGrid.at(y).at(x).oneTimeUse)
				rectangleGrid.at(y).at(x).drawable = false;
		}
	}
	al_unlock_bitmap(screenBuffor);

	al_set_target_bitmap(DISPLAY_BITMAP);
	al_draw_bitmap(screenBuffor, 0, 0, 0);
}