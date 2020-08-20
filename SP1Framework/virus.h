#pragma once
#include "entity.h"
#include <random>

class virus : public entity
{
private:
	static int totalvirus;
public:
	virus(float spd, map&);
	virus(position, float);
	~virus();

	//getters
	static int gettotal();
	//setters

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void OnTriggerEnter(position, map&);
};

