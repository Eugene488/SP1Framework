#pragma once
#include "entity.h"


class player :public entity
{
private:
public:
	player();
	~player();
	player(position, int hp, float spd, image);

	//getters

	//setters

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
};

