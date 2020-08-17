#include "position.h"

position::position(): x(-99), y(-99) {}
position::position(int x, int y) : x(x), y(y) {}
position::~position() {
	//do nothing
}

//gettters
int position::get(char xy) {
	if (xy == 'x')
	{
		return x;
	}
	else if (xy == 'y')
	{
		return y;
	}
}

//setters
void position::set(char xy, int value) {
	if (xy == 'x')
	{
		x = value;
	}
	else if (xy == 'y')
	{
		y = value;
	}
}
