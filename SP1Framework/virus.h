#pragma once
#include "entity.h"
#include <random>

class virus : public entity
{
private:
<<<<<<< HEAD
	static int totalvirus;
=======

>>>>>>> map-building
public:
	virus(float spd, map&);
	virus(position, float);
	~virus();

	//getters
<<<<<<< HEAD
	static int gettotal();
	//setters

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
=======

	//setters

	//other methods
	void move(map&, WORD solids[], int listsize);
>>>>>>> map-building
	void OnTriggerEnter(position, map&);
};

