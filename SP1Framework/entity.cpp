#include "entity.h"

entity::entity():pos(-1,-1){}

entity::~entity()
{
	//do nothing
}

entity::entity(position pos, image image, float spd): pos(pos), imagey(image), spd(spd), spdtimer(0), previmg(image::image(NULL, 0)){}

//getters
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

float entity::getspd() {
	return spd;
}
float entity::getspdtimer() {
	return spdtimer;
}

//setters
void entity::setpos(position pos, map& map) {
	this->pos = pos;
	map.setmapposition(pos, imagey);
}

void entity::setimage(image imagey) {
	this->imagey = imagey;
}

void entity::setspd(float spd) {
	this->spd = spd;
}

void entity::setspdtimer(float spdtimer) {
	this->spdtimer = spdtimer;
}

//other methods
bool entity::collisiondetection(WORD solids[], int listsize, position& futurloc, map& g_map) {
	for (int i = 0; i < listsize; i++)
	{
		if (static_cast<WORD>(g_map.getmapposition(futurloc).getcolour()) == static_cast<WORD>(solids[i]))
		{
			return false;
		}
	}
	return true;
}