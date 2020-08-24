#include "player.h"

player::player(): entity(position(0,0), image('P', 18), spd, 5, "player"), invulntime(0.5f), invulntimer(0){}
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
float player::getfireratetimer() {
	return fireratetimer;
}
float player::getfirerate() {
	return firerate;
}

//setters
void player::setItime(float time) {
	invulntime = time;
}
void player::setItimer(float timer) {
	invulntimer = timer;
}
void player::setfireratetimer(float time) {
	fireratetimer += time;
}
void player::setfirerate(float time) {
	firerate += time;
}
//other methods
void player::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	//TODO
}
void player::takedmg(int dmg) {
	if (invulntimer >= invulntime)
	{
		hp -= dmg;
		invulntimer = 0;
	}
}
void player::updatetimers(float dt) {
	spdtimer += dt;
	if (invulntimer < invulntime)
	{
		invulntimer += dt;
	}
	if (fireratetimer < firerate)
	{
		fireratetimer += dt;
	}
}