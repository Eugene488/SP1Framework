#pragma once
#include <string>
using namespace std;
class entity
{
protected:
	int x;
	int y;
	string name;
public:
	entity();
	~entity();
	entity(int x, int y);
	int getx();
	int gety();
	void setx(int xnumber);
	void sety(int ynumber);

	void spawn();

	void spawnout();
	void killed_or_eaten();
	
	virtual string getname();

};

