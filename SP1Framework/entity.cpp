#include "entity.h"

entity::entity():pos(-1,-1), hp(1), spd(5), spdtimer(0){}

entity::~entity()
{
	//do nothing
}

entity::entity(position pos, image image, float spd, int hp, string name): pos(pos), imagey(image), spd(spd), spdtimer(0), previmg(image::image(NULL, 0)), hp(hp), name(name){}

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
int entity::gethp() {
	return hp;
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
void entity::sethp(int hp) {
	this->hp = hp;
}

//other methods
bool entity::collisiondetection(WORD solids[], int listsize, position& futurloc, map& g_map) {
	for (int i = 0; i < listsize; i++)
	{
		if (static_cast<WORD>(g_map.getmapposition(futurloc).getcolour()) == static_cast<WORD>(solids[i]))
		{
			return false;
			break;
		}
	}
	return true;
}
void entity::OnTriggerEnter(position, map&) {
	//do nothing
}
void entity::die(map& g_map) {
	g_map.setmapposition(pos, image());
	delete this;
}