#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

class image
{
private:
	char text;
	WORD colour;
public:
	image();
	image(char, WORD);
	~image();
	
	//getters
	char gettext();
	WORD getcolour();
	
	//setters
	void settext(char);
	void setcolour(WORD);
};

