#pragma once
#include "entity.h"
#include <random>

class virus : public entity
{
private:
	static int totalvirus;
	static int maxvirus;
public:
	virus(float spd, map&);
	virus(position position, float spd, map& g_map);
	~virus();

	//getters
	static int gettotal();
	static int getmax();
	//setters
	static void setmax(int max);

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void OnTriggerEnter(position, map&);
};

