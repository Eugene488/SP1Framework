#include "player.h"

player::player(): entity(position(0,0), image('P', 18)), speed(0.1f), hp(3) {}
player::~player() {
	//do nothing
}
player::player(position pos, int hp, float spd, image imagey):entity(pos, imagey), hp(hp), speed(spd) {}

//getters
int player::gethp() {
	return hp;
}
float player::getspd() {
	return speed;
}
//setters
void player::sethp(int value) {
	hp = value;
}
void player::setspd(float value) {
	speed = value;
}

//other methods
void player::move(position futurloc) {
	//TODO
}