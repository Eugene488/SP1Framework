#include "boss.h"

boss::boss(position position, float spd, map& g_map):entity(position, image(NULL, 0), 1, 100, "???"), phase(0) {}
boss::~boss() {
	//do nothing
}

void boss::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	//TODO
}
void boss::update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* player) {
	setimage(image(rand() % 200 - 100, rand() % 255));
	setpos(pos, g_map);
}
void boss::updatetimers(float dt) {
	//TODO
}