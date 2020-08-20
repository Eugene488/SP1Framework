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
void fire::move(map& g_map, WORD solids[], int listsize) {
	
}

void fire::die(map& g_map, map& bg_map, map& bgc_map) {
	bg_map.setmapposition(pos, image(NULL, 0));
	bgc_map.setmapposition(pos, previmg);
}