#include "Mask.h"
#include <iostream>
#include <Windows.h>
using namespace std;
Mask::Mask()
{
	WORD charColor = 0x0B;

}


void Mask::gotoxy(short x, short y)
{
	COORD scrn;
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = x; scrn.Y = y;
	SetConsoleCursorPosition(hConsoleOutput, scrn);
}

void Mask::setMaskX(int x)
{
	posx = x;
}

void Mask::setMaskY(int y)
{
	posy = y;
}

int Mask::getMaskX()
{
	return posx;
}

int Mask::getMaskY()
{
	return posy;
}
