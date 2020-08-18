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
public:
	entity();
	~entity();
	entity(position, image);

	position getpos();
	string getname();
	image getimage();

	void setpos(position, map&);
	void setimage(image);
};

