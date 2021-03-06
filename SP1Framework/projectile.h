#pragma once
#include "entity.h"
class projectile :public entity
{
private:
	float truex;
	float truey;
	float directionx;
	float directiony;
	string target;
	int dmg;
	image trailimg;
	string trailmap;
public:
	projectile(position startpos, position endpos, image projectile_image, float spd, string name, map& g_map, string target, int dmg, image trailimg, string trailmap = "bg");
	~projectile();

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
};

