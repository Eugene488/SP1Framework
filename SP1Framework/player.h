#pragma once
#include "entity.h"


class player :public entity
{
private:
	int hp;
public:
	player();
	~player();
	player(position, int hp, float spd, image);

	//getters
	int gethp();
	//setters
	void sethp(int value);

	//other methods
	void move(map&, WORD solids[], int listsize);
};

