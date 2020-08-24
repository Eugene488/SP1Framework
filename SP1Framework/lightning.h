#pragma once
#include "entity.h"
#include "fire.h"

class lightning : public entity
{
private:

public:
	lightning(position pos, map& fg_map);
	~lightning();
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void die(map& g_map, map& bg_map, map& bgc_map);
};

