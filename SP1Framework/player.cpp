#include "player.h"

<<<<<<< HEAD
player::player(): entity(position(0,0), image('P', 18), spd, 3, "player"), invulntime(0.5f), invulntimer(0){}
player::~player() {
	//do nothing
}
player::player(position pos, int hp, float spd, image imagey):entity(pos, imagey, spd, hp, "player"), invulntime(0.5f), invulntimer(0) {}

//getters
float player::getItime() {
	return invulntime;
}
float player::getItimer() {
	return invulntimer;
}

//setters
void player::setItime(float time) {
	invulntime = time;
}
void player::setItimer(float timer) {
	invulntimer = timer;
}

//other methods
void player::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	//TODO
}
void player::takedmg(int dmg) {
	if (invulntimer >= 0.5f)
	{
		hp -= dmg;
		invulntimer = 0;
	}
=======
player::player(): entity(position(0,0), image('P', 18), spd, 3, "player"){}
player::~player() {
	//do nothing
}
player::player(position pos, int hp, float spd, image imagey):entity(pos, imagey, spd, hp, "player"){}

//other methods
void player::move(map& g_map, WORD solids[], int listsize) {
	//TODO
>>>>>>> map-building
}