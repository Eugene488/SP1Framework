#pragma once
#include "entity.h"
class player :public entity
{
private:
	int hp;
	float speed;
public:
	player();
	~player();
	player(position, int hp, float spd, image);

	//getters
	int gethp();
	float getspd();
	//setters
	void sethp(int value);
	void setspd(float value);

	//other methods
	void move(position futurloc);
};

