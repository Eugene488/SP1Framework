#pragma once
#include "entity.h"
#include <random>

class virus : public entity
{
private:

public:
	virus(float spd, map&);
	virus(position, float);
	~virus();

	//getters

	//setters

	//other methods
	void move(map&, WORD solids[], int listsize);
	void OnTriggerEnter(position, map&);
};

