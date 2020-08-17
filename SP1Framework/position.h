#pragma once
class position
{
private:
	int x;
	int y;
public:
	position();
	position(int x, int y);
	~position();

	//gettters
	int get(char xy);

	//setters
	void set(char xy, int value);

};

