#include "boss.h"


const int boss::maxentities = 2000;

extern void backgroundchange(string bg, string bgc, string fg);
extern map fg_map;
extern int scene;
extern float cutscenetimer;
extern image wallskin;

boss::boss(position position, float spd, map& g_map):entity(position, image(NULL, 0), 1, 10, "???") {}
boss::~boss() {
	//do nothing
}
//setters
void boss::setphase(int phase) {
	phasechange(phase);
}

//other methods
void boss::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	if (phase == 1)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		fg_map.clearmap();
		wallskin = image(-78, 6 + 224);
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
	else if (phase == 2)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		fg_map.clearmap();
		spd = 100;
	}
	else if (phase == 3)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		wallskin = image(NULL, 0);
		backgroundchange("purple", "clear", "clear");
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
		/*for (int i = 0; i < 3; i++)
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
		}*/
		//replace a random spot in block with lightning
		int x = rand() % 3 - 1;
		int y = rand() % 3 - 1;
		for (int i = 0; i < maxentities; i++)
		{
			if (entities[i] == NULL)
			{
				if (x + playerpos.get('x') > 0 && y + playerpos.get('y') > 0 && (x != 0 || y != 0))
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
		//boss goes to random position
		g_map.setmapposition(pos, image(NULL, 0));
		setpos(position(rand() % 70 + 5, rand() % 20 + 2), g_map);
		//spawn barricades
		for (int i = 0; i < 500; i++)
		{
			int x = rand() % 78 + 1;
			int y = rand() % 23 + 1;
			int lightninginstead = rand() % 100;
			if (g_map.getmapposition(position(x, y)).gettext() != static_cast<char>(1) || (x != pos.get('x') && y != pos.get('y')))
			{
				for (int i = 0; i < maxentities; i++)
				{
					if (entities[i] == NULL)
					{
						if (lightninginstead == 1)
						{
							entities[i] = new lightning(position(x, y), fg_map);
							break;
						}
						else
						{
							entities[i] = new boulder(position(x, y), 1, g_map);
							break;
						}
					}
				}
			}
		}
		spd = 15;
	}
	else if (phase == 4)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		fg_map.clearmap();
		spd = 1000000;
	}
	else if (phase == 5)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		wallskin = image('x', 4);
		backgroundchange("space", "space", "clear");
		//spawn random barricades
		for (int i = 0; i < 25; i++)
		{
			int x = rand() % 78 + 1;
			int y = rand() % 23 + 1;
			if (g_map.getmapposition(position(x, y)).gettext() != static_cast<char>(1) || (x != pos.get('x') && y != pos.get('y')))
			{
				for (int i = 0; i < maxentities; i++)
				{
					if (entities[i] == NULL)
					{
						entities[i] = new boulder(position(x, y), 1, g_map);
						break;
					}
				}
			}
		}
		//spawn projectiles vertically
		for (int i = 0; i < 50; i++)
		{
			for (int i = 0; i < MAXENTITY; i++)
			{
				if (entities[i] == NULL)
				{
					int x = rand() % 78 + 1;
					entities[i] = new projectile(position(x, 1), position(x, 2), image(-17, 5 + 208), 0.2f, "poison needle", g_map, "", 1, image(NULL, 0));
					break;
				}
			}
		}
		//spawn projectiles horizontally
		for (int i = 0; i < 20; i++)
		{
			for (int i = 0; i < MAXENTITY; i++)
			{
				if (entities[i] == NULL)
				{
					int y = rand() % 23 + 1;
					float spd = rand() % 30;
					entities[i] = new projectile(position(1, y), position(2, y), image(-17, 5 + 208), (spd/100), "poison needle", g_map, "", 1, image(NULL, 0));
					break;
				}
			}
		}
		for (int i = 0; i < 20; i++)
		{
			for (int i = 0; i < MAXENTITY; i++)
			{
				if (entities[i] == NULL)
				{
					int y = rand() % 23 + 1;
					float spd = rand() % 30;
					entities[i] = new projectile(position(78, y), position(77, y), image(-17, 5 + 208), (spd/100), "poison needle", g_map, "", 1, image(NULL, 0));
					break;
				}
			}
		}
		//move boss to random location
		g_map.setmapposition(pos, image(NULL, 0));
		setpos(position(rand() % 70 + 5, rand() % 20 + 2), g_map);
		spd = 25;
	}
	else if (phase == 6)
	{
		clearentities(entities, g_map, bg_map, bgc_map);
		fg_map.clearmap();
		spd = 1000000;
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
			spd = 0;
		}
	}
	else if (phase == 3)
	{
		if (player->getpos().get('x') == pos.get('x') && player->getpos().get('y') == pos.get('y'))
		{
			hp -= 1;
			spd = 0;
			backgroundchange("", "", "white");
		}
		if (hp <= 5)
		{
			phasechange(4);
		}
		stringstream ss;
		ss.str("");
		ss << "boss hp:";
		for (int i = 0; i < hp - 5; i++)
		{
			ss << " " << char(3);
		}
		fg_map.setmapposition(position(27, 24), ss.str(), static_cast<WORD>(2));
	}
	else if (phase == 5)
	{
		if (player->getpos().get('x') == pos.get('x') && player->getpos().get('y') == pos.get('y'))
		{
			phasetimer += 5;
			spdtimer = 0;
			spd = 0.5f;
			g_map.setmapposition(pos, image(NULL, 0));
			setpos(position(rand() % 70 + 5, rand() % 20 + 2), g_map);
		}
		if (phasetimer >= 90)
		{
			phasechange(6);
		}
		stringstream ss;
		ss.str("");
		ss << "boss hp: ???";
		fg_map.setmapposition(position(27, 24), ss.str(), static_cast<WORD>(2));
		if (spdtimer >= spd - 0.2f)
		{
			backgroundchange("", "", "white");
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
		scene = 3;
		cutscenetimer = 0;
	}
	else if (phase == 3)
	{
		spd = 1;
		spdtimer = 0;
	}
	else if (phase == 4)
	{
		scene = 5;
		spd = 0;
		cutscenetimer = 0;
	}
	else if (phase == 5)
	{
		spd = 2;
	}
	else if (phase == 6)
	{
		scene = 7;
		spd = 0;
		cutscenetimer = 0;
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