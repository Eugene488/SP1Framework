#pragma once
#include "game.h"
#include "entity.h"
#include "map.h"
class Mask :  public entity
{
private:
	int posx;
	int posy;
public:
	Mask();
	void gotoxy(short x,short y);
	void setMaskX(int x);
	void setMaskY(int y);
	int getMaskX();
	int getMaskY();
};

