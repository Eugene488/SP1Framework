#pragma once
#include "entity.h"


class player :public entity
{
private:
<<<<<<< HEAD
	float invulntime;
	float invulntimer;
=======
>>>>>>> map-building
public:
	player();
	~player();
	player(position, int hp, float spd, image);

	//getters
<<<<<<< HEAD
	float getItime();
	float getItimer();
	//setters
	void setItime(float time);
	void setItimer(float timer);
	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void takedmg(int dmg);
=======

	//setters

	//other methods
	void move(map&, WORD solids[], int listsize);
>>>>>>> map-building
};

