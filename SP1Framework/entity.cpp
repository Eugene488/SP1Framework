#include "entity.h"

entity::entity():pos(-1,-1), hp(1), spd(5), spdtimer(0), previmg(image(NULL, 0)){}

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
void entity::die(map& g_map, map& bg_map, map& bgc_map) {
	g_map.setmapposition(pos, image(NULL, 0));
	delete this;
}

void entity::update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* player) {
	//do nothing
}

string entity::triggerdetection(map& g_map, map& bg_map, map& bgc_map, position position_to_check, string map_to_check) {
	if (map_to_check == "g_map")
	{
		image gimg = g_map.getmapposition(position_to_check);
		if (gimg.getcolour() == static_cast<WORD>(213))
		{
			return "virus";
		}
		else if (gimg.getcolour() == static_cast<WORD>(0) && gimg.gettext() == static_cast<char>(1))
		{
			return "player";
		}
		else if (gimg.getcolour() == static_cast<WORD>(0x0B))
		{
			return "mask";
		}
	}
	else if (map_to_check == "bgc_map")
	{
		image bgcimg = bgc_map.getmapposition(position_to_check);
		if (bgcimg.gettext() == static_cast<char>(-21))
		{
			return "fire";
		}
	}
	else if (map_to_check == "bg_map")
	{
		image bgimg = bg_map.getmapposition(position_to_check);
		if (bgimg.getcolour() == static_cast<WORD>(144))
		{
			return "water";
		}
	}
	return "none";
}

void entity::updatetimers(float dt) {
	spdtimer += dt;
}