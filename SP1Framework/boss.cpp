#include "boss.h"
#include "lightning.h"
#include "virus_spawner.h"
#include "boulder.h"

const int boss::maxentities = 500;

extern void backgroundchange(string bg, string bgc, string fg);
extern map fg_map;

boss::boss(position position, float spd, map& g_map):entity(position, image(NULL, 0), 1, 100, "???") {}
boss::~boss() {
	//do nothing
}

void boss::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	if (phase == 1)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		fg_map.clearmap();
		backgroundchange("nature", "nature", "");
		position futurloc;
		for (int i = 0; i < 10; i++)
		{
			futurloc = position(rand() % 70 + 5, rand() % 20 + 2);
			if (g_map.getmapposition(futurloc).gettext() != static_cast<char>(1))
			{
				for (int i = 0; i < maxentities; i++)
				{
					if (entities[i] == NULL)
					{
						entities[i] = new lightning(futurloc, fg_map);
						break;
					}
				}
			}
		}
		for (int i = 0; i < 7; i++)
		{
			futurloc = position(rand() % 70 + 5, rand() % 20 + 2);
			if (g_map.getmapposition(futurloc).gettext() != static_cast<char>(1))
			{
				for (int i = 0; i < maxentities; i++)
				{
					if (entities[i] == NULL)
					{
						entities[i] = new virus_spawner(futurloc, 2, g_map);
						break;
					}
				}
			}
		}
	}
	if (phase == 2)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		backgroundchange("nature", "nature", "clear");
		position playerpos = entities[0]->getpos();
		//spawn boulders to surrond player
		for (int i2 = 0; i2 < 3; i2++)
		{
			for (int i = 0; i < maxentities; i++)
			{
				if (entities[i] == NULL)
				{
					if (playerpos.get('x') - 1 > 0 && playerpos.get('y') - 1 + i2 > 0)
					{
						entities[i] = new boulder(position(playerpos.get('x') - 1, playerpos.get('y') - 1 + i2), 1, g_map);
						break;
					}
				}
			}
			for (int i = 0; i < maxentities; i++)
			{
				if (entities[i] == NULL)
				{
					if (playerpos.get('x') + 1 > 0 && playerpos.get('y') - 1 + i2 > 0)
					{
						entities[i] = new boulder(position(playerpos.get('x') + 1, playerpos.get('y') - 1 + i2), 1, g_map);
						break;
					}
				}
			}
			for (int i = 0; i < maxentities; i++)
			{
				if (entities[i] == NULL)
				{
					if (i != 1)
					{
						if (playerpos.get('y') - 1 + i2 > 0)
						{
							entities[i] = new boulder(position(playerpos.get('x'), playerpos.get('y') - 1 + i2), 1, g_map);
						}
					}
					break;
				}
			}
		}
		//spawn virus spawners at random spots to surround player
		for (int i = 0; i < 3; i++)
		{
			int x = (rand() % 3 - 1) * 2;
			int y = (rand() % 3 - 1) * 2;
			for (int i = 0; i < maxentities; i++)
			{
				if (entities[i] == NULL)
				{
					if (x + playerpos.get('x') > 0 && y + playerpos.get('y') > 0)
					{
						entities[i] = new virus_spawner(position(x + playerpos.get('x'), y + playerpos.get('y')), 3, g_map);
						break;
					}
				}
			}
		}
		//replace a random spot in block with lightning
		int x = rand() % 3 - 1;
		int y = rand() % 3 - 1;
		for (int i = 0; i < maxentities; i++)
		{
			if (entities[i] == NULL)
			{
				if (x + playerpos.get('x') > 0 && y + playerpos.get('y') > 0)
				{
					entities[i] = new lightning(position(x + playerpos.get('x'), y + playerpos.get('y')), fg_map);
					break;
				}
			}
		}
		//spawning the border walls in case it gets replaced
		for (int i = 0; i < 25; i++)
		{
			g_map.setmapposition(position(79, i), image(NULL, 240));
			g_map.setmapposition(position(0, i), image(NULL, 240));
		}
		for (int i = 0; i < 79; i++)
		{
			g_map.setmapposition(position(i, 24), image(NULL, 240));
			g_map.setmapposition(position(i, 0), image(NULL, 240));
		}
	}
}
void boss::update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* player) {
	setimage(image(rand() % 200 - 100, rand() % 255));
	for (int i = 0; i < 3; i++)
	{
		name[i] = rand() % 200;
	}
	setpos(pos, g_map);
	if (phase == 0)
	{
		if (phasetimer >= 2.2f)
		{
			backgroundchange("nature", "nature", "clear");
			phasechange(1); //change for testing
		}
		else if (phasetimer >= 2)
		{
			backgroundchange("", "", "white");
		}
	}
	else if (phase == 1)
	{
		if (spdtimer >= spd-0.2f)
		{
			backgroundchange("", "", "white");
		}
		if (phasetimer >= 58)
		{
			phasechange(2);
		}
	}
	else if (phase == 2)
	{
		if (spdtimer >= spd - 0.2f)
		{
			backgroundchange("", "", "white");
		}
		if (phasetimer >= 30)
		{
			extern int maplevel;
			maplevel = 6;
			//TODOphasechange(3);
		}
	}
}
void boss::updatetimers(float dt) {
	spdtimer += dt;
	phasetimer += dt;
}
void boss::phasechange(int phasetochangeto) {
	phase = phasetochangeto;
	phasetimer = 0;
	if (phase == 1)
	{
		spd = 30;
		spdtimer = 25;
		virus::setmax(200);
	}
	else if (phase == 2)
	{
		spd = 7;
		spdtimer = 0;
	}
}
void boss::clearentities(entity** entities, map& g_map, map& bg_map, map& bgc_map) {
	for (int i = 2; i < maxentities; i++)
	{
		if (entities[i] != NULL)
		{
			entities[i]->die(g_map, bg_map, bgc_map);
			entities[i] = NULL;
		}
	}
}