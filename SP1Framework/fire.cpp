#include "fire.h"

const WORD fire::states_bg_colours[3] = { 64, 192, 224 };

fire::fire(float spd, map& g_map, map& bg_map, map& bgc_map) {
	this->previmg = image(NULL, 96);
	this->spdtimer = 0;
	this->spd = spd;
	this->hp = 3;
	this->name = "fire";
	setimage(image(-21, 15));
	while (true) {
		position spawnloc = position(rand() % (g_map.getmapsize('x') - 3) + 1, rand() % (g_map.getmapsize('y') - 2) + 1);
		if (g_map.getmapposition(spawnloc).gettext() == NULL && static_cast<WORD>(g_map.getmapposition(spawnloc).getcolour()) == static_cast<WORD>(0))
		{
			setpos(spawnloc, bgc_map);
			bg_map.setmapposition(pos, image(NULL, 224));
			break;
		}
	}
}
fire::fire(position pos, float spd, int hp, map& bgc_map, map& bg_map) :entity(pos, image(-21, 15), spd, hp, "fire") {
	this->previmg = image(NULL, 96);
	if (hp == 2)
	{
		imagey = image(-21, 14);
	}
	else if (hp == 1)
	{
		imagey = image(-21, 12);
	}
	setpos(pos, bgc_map);
	bg_map.setmapposition(pos, image(NULL, states_bg_colours[hp-1]));
}
fire::~fire() {
	//do nothing
}

//other methods
void fire::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	position futurloc;
	int statechange = 0;
	for (int i = 0; i < 8; i++)
	{
		futurloc = pos;
		switch (i)
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
		if (collisiondetection(solids, listsize, futurloc, g_map))
		{
			if (triggerdetection(g_map, bg_map, bgc_map, futurloc, "bg_map") == "water")
			{
				//do nothing
			}
			else if (bgc_map.getmapposition(futurloc).gettext() != -21)
			{
				statechange = rand() % 2 - 1;
				int hpc = hp + statechange;
				if (hpc > 3)
				{
					hpc = 3;
				}
				for (int i = 0; i < MAXENTITY; i++)
				{
					if (entities[i] == NULL)
					{
						entities[i] = new fire(futurloc, rand() % 5 + 2, hpc, bgc_map, bg_map);
						break;
					}
				}
			}
		}
	}
	hp--;
	//updating map
	if (hp == 2)
	{
		imagey = image(-21, 14);
	}
	else if (hp == 1)
	{
		imagey = image(-21, 12);
	}
	setpos(pos, bgc_map);
	bg_map.setmapposition(pos, image(NULL, states_bg_colours[hp - 1]));
}

void fire::die(map& g_map, map& bg_map, map& bgc_map) {
	bgc_map.setmapposition(pos, image(NULL, 0));
	bg_map.setmapposition(pos, image(NULL, 96));
}

void fire::update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* player) {
	if (triggerdetection(g_map, bg_map, bgc_map, pos, "bg_map") == "water")
	{
		hp = 0;
	}
}