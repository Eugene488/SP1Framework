#include "virus_spawner.h"

virus_spawner::virus_spawner(position position, float spd, map& g_map): entity(position, image('V', 2 + 80), spd, 1, "virus spawner"){
	setpos(pos, g_map);
}
virus_spawner::~virus_spawner() {
	//do nothing
}

//other methods
void virus_spawner::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	spd = rand() % 1 + 0.1f;
	if (virus::gettotal() < virus::getmax())
	{
		int dir = rand() % 8 + 1;
		position futurloc = pos;
		switch (dir)
		{
		case 1:
			futurloc.set('y', futurloc.get('y') - 1); break;
		case 2:
			futurloc.set('x', futurloc.get('x') + 1); futurloc.set('y', futurloc.get('y') - 1); break;
		case 3:
			futurloc.set('x', futurloc.get('x') + 1); break;
		case 4:
			futurloc.set('x', futurloc.get('x') + 1); futurloc.set('y', futurloc.get('y') + 1); break;
		case 5:
			futurloc.set('y', futurloc.get('y') + 1); break;
		case 6:
			futurloc.set('x', futurloc.get('x') - 1); futurloc.set('y', futurloc.get('y') + 1); break;
		case 7:
			futurloc.set('x', futurloc.get('x') - 1); break;
		case 8:
			futurloc.set('x', futurloc.get('x') - 1); futurloc.set('y', futurloc.get('y') - 1); break;
		}
		if (g_map.getmapposition(futurloc).gettext() == NULL && g_map.getmapposition(futurloc).getcolour() == 0)
		{
			for (int i = 0; i < MAXENTITY; i++)
			{
				if (entities[i] == NULL)
				{
					entities[i] = new virus(futurloc, 0.5f, g_map);
					break;
				}
			}
		}
	}
}