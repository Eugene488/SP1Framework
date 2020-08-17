#pragma once
#include <string>
#include "position.h"

using namespace std;
class entity
{
protected:
	position pos;
	string name; //name and description(for mouse-over tooltip)
public:
	entity();
	~entity();
	entity(position);
	position getpos();
	void setpos(char xy, int value);
	string getname();
};

