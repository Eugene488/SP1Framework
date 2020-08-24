#pragma once
#include "entity.h"


class player :public entity
{
private:
	float invulntime;
	float invulntimer;
	float firerate = 0.1f;
	float fireratetimer = 0;
public:
	player();
	~player();
	player(position, int hp, float spd, image);

	//getters
	float getItime();
	float getItimer();
	float getfireratetimer();
	float getfirerate();
	//setters
	void setItime(float time);
	void setItimer(float timer);
	void setfireratetimer(float time);
	void setfirerate(float time);
	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void takedmg(int dmg);
	void updatetimers(float dt);
};

