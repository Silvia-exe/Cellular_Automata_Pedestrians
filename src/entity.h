#ifdef USE_ALLEGRO
#include<allegro5/allegro.h>


struct entity
{
	int value;

	ALLEGRO_COLOR col;

	bool drawable;
	bool oneTimeUse;

	entity()
	{
		value = 0;
		col = al_map_rgb(255, 255, 255);
		drawable = false;
		oneTimeUse = true;
	}
};
#endif