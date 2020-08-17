#include "entity.h"

entity::entity()
{
	x = 1;
	y = 1;
	name = "";
}

entity::~entity()
{
}

entity::entity(int x, int y)
{
	this->x = x;
	this->y = y;
}

int entity::getx()
{
	return x;
}

int entity::gety()
{
	return y;
}

void entity::setx(int xnumber)
{
	x = xnumber;
}

void entity::sety(int ynumber)
{
	y = ynumber;
}

string entity::getname()
{
	return name;
}

void entity::spawn()
{
	for (int x = 0; x < 2; x++)
	{
		int xLoc = rand() % 18 + 1; //1-17+1
		int yLoc = rand() % 18 + 1;
		setx(xLoc);
		sety(yLoc);


	}

}





void entity::spawnout()
{
	for (int x = 0; x < 2; x++)
	{
		int xLoc = rand() % -300;
		int yLoc = rand() % -400;
		setx(xLoc);
		sety(yLoc);


	}
}

void entity::killed_or_eaten()
{
	x = -1;
	y = -1;



}
