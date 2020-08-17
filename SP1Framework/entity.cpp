#include "entity.h"

entity::entity():pos(-1,-1){}

entity::~entity()
{
	//do nothing
}

entity::entity(position pos, image image): pos(pos), imagey(image){}

position entity::getpos()
{
	return pos;
}

string entity::getname()
{
	return name;
}

image entity::getimage() {
	return imagey;
}

void entity::setpos(char xy, int value, map map) {
	if (xy == 'x')
	{
		pos.set('x', value);
	}
	else if (xy == 'y')
	{
		pos.set('y', value);
	}
	map.setmapposition(pos, imagey);
}