#include "virus.h"

virus::virus(float spd, map& g_map){
	this->spdtimer = 0;
	this->spd = spd;
	this->hp = 1;
	this->name = "virus";
	setimage(image(15, 5));
	while (true) {
		position spawnloc = position(rand() % (g_map.getmapsize('x')-3) + 1, rand() % (g_map.getmapsize('y')-2) + 1);
		if (g_map.getmapposition(spawnloc).gettext() == NULL && static_cast<WORD>(g_map.getmapposition(spawnloc).getcolour()) == static_cast<WORD>(0))
		{
			setpos(spawnloc, g_map);
			break;
		}
	}
}
virus::virus(position pos, float spd):entity(pos, image(15, 5), spd, 1, "virus"){}
virus::~virus() {
	//do nothing
}

//getters

//setters

//other methods
void virus::move(map& g_map, WORD solids[], int listsize) { //1:up 2:upright 3:right...
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
	if (collisiondetection(solids, listsize, futurloc, g_map))
	{
		prevloc = getpos();
		g_map.setmapposition(prevloc, previmg);
		previmg = image(NULL, 0);
		setpos(futurloc, g_map);
	}
}

void virus::OnTriggerEnter(position, map&) {
	//TODO player detection
}