#pragma once
#include "entity.h"
class boulder : public entity
{
private:
	float pushtime = 1;
	float pushtimer = 0;
public:
	boulder(position position, float spd, map& g_map);
	~boulder();

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void updatetimers(float dt);
};

