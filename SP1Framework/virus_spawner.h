#pragma once
#include "entity.h"
#include "virus.h"

class virus_spawner :public entity
{
private:

public:
	virus_spawner(position position, float spd, map& g_map); //spd is spawnspd
	~virus_spawner();

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
};


