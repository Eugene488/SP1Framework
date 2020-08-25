#pragma once
#include "entity.h"
class boss :public entity
{
public:
	int phase = 0; //debug change later
private:
	float phasetimer = 0;
	static const int maxentities;
public:
	boss(position position, float spd, map& g_map);
	~boss();

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* player);
	void updatetimers(float dt);
	void phasechange(int phase);
	void clearentities(entity** entities, map& g_map, map& bg_map, map& bgc_map);
};

