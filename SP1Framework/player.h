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
	void move(map&, WORD solids[], int listsize);
};

