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
	int hp;
public:
	entity();
	virtual ~entity();
	entity(position pos, image image, float spd, int hp, string name);

	//getters
	position getpos();
	string getname();
	image getimage();
	float getspd();
	float getspdtimer();
	int gethp();

	//setters
	void setpos(position, map&);
	void setimage(image);
	void setspd(float);
	void setspdtimer(float);
	void sethp(int);

	//other methods
<<<<<<< HEAD
	virtual void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) = 0;
	bool collisiondetection(WORD[], int, position&, map&);
	virtual void OnTriggerEnter(position, map&);
	virtual void die(map& g_map, map& bg_map, map& bgc_map);
=======
	virtual void move(map&, WORD solids[], int listsize) = 0;
	bool collisiondetection(WORD[], int, position&, map&);
	virtual void OnTriggerEnter(position, map&);
	void die(map&);
>>>>>>> map-building
};

