#pragma once
#include <string>
#include "map.h"


using namespace std;
class entity
{
protected:
	position pos;
	string name; //name and description(for mouse-over tooltip)
	image imagey;
	float spd; //how fast the entity moves (in seconds per tile)
	float spdtimer;
	image previmg;
	position prevloc;
public:
	entity();
	~entity();
	entity(position, image, float);

	//getters
	position getpos();
	string getname();
	image getimage();
	float getspd();
	float getspdtimer();

	//setters
	void setpos(position, map&);
	void setimage(image);
	void setspd(float);
	void setspdtimer(float);

	//other methods
	virtual void move(map&, WORD solids[], int listsize) = 0;
	bool collisiondetection(WORD[], int, position&, map&);
};

