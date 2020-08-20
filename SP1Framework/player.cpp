#include "player.h"

player::player(): entity(position(0,0), image('P', 18), spd, 3, "player"){}
player::~player() {
	//do nothing
}
player::player(position pos, int hp, float spd, image imagey):entity(pos, imagey, spd, hp, "player"){}

//other methods
void player::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	//TODO
}