#pragma once
#include "entity.h"
#include <random>

class virus : public entity
{
private:

public:
	virus(position, float, image);
	~virus();

	//getters

	//setters

	//other methods
	void move(map&, WORD solids[], int listsize);
};

