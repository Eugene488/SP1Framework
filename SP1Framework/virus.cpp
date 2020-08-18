#include "virus.h"

virus::virus(position pos, float spd, image img):entity(pos, img, spd){}
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
	if (collisiondetection(solids, listsize, getpos(), g_map))
	{
		prevloc = getpos();
		g_map.setmapposition(prevloc, previmg);
		previmg = g_map.getmapposition(futurloc);
		setpos(futurloc, g_map);
	}
}