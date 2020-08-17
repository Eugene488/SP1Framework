#include "entity.h"

entity::entity():pos(-1,-1){}

entity::~entity()
{
}

entity::entity(position pos): pos(pos){}

position entity::getpos()
{
	return pos;
}

void entity::setpos(char xy, int value) {
	if (xy == 'x')
	{
		pos.set('x', value);
	}
	else if (xy == 'y')
	{
		pos.set('y', value);
	}
}

string entity::getname()
{
	return name;
}
