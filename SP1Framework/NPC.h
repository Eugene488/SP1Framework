#pragma once
#include "entity.h"
class NPC :public entity
{
private:
	string text;
public:
	NPC(position position, int hp, float spd, image image, string texts, string name);
	~NPC();
};

