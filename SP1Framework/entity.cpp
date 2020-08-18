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

void entity::setpos(position pos, map& map) {
	this->pos = pos;
	map.setmapposition(pos, imagey);
}