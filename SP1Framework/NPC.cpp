#include "NPC.h"
NPC::NPC(position position, int hp, float spd, image image, string texts, string name) :entity(position, image, spd, hp, name), text(texts) {}
NPC::~NPC() {
	//do nothing
}

