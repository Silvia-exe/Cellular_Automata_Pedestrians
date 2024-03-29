#ifdef USE_ALLEGRO
#include <iostream>


#include"graphicsHandler.h"

void randomMovementTest(graphicsHandler& testHandler, point<int> max, int movementsAmount)
{
	entity testEntity;
	testEntity.col = al_map_rgb(255, 0, 0);
	testEntity.drawable = true;

	testHandler.insertEntityToGrid(testEntity, rand() % max.x, rand() % max.y);

	for (int i = 0; i < movementsAmount; i++)
	{
		
	}
}

int main()
{
	al_init();
	al_init_primitives_addon();
	
	graphicsHandler a(20, 20, 10);

	ALLEGRO_DISPLAY* display = al_create_display(800, 600);

	entity testEntity;
	testEntity.col = al_map_rgb(255, 0, 0);
	testEntity.drawable = true;

	a.insertEntityToGrid(testEntity, 10, 10);
	a.drawTheGrid();
	al_flip_display();

	al_rest(10);

	return 0;
}
#endif
