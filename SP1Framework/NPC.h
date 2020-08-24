#pragma once
#include "entity.h"
#include "Framework/console.h"

class NPC :public entity
{
private:
	string dialogue; // use '`'(tilde) to indicate a new line
	string currentsentences[3];
	int currentsentenceposition = 0;
	int currentsentencenumber = 1;
	float textdisappeartime = 0;
	float textdisappeartimer = 0;
	float texttime = 0.1f; //time it takes for each char to load
	float texttimer = 0;
public:
	NPC(position position, int hp, float spd, image image, string texts, string name, map& g_map);
	~NPC();

	//other methods
	void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
	void displaytext(string* sentence, int lines, WORD textcolour, map& fg_map);
	void update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* g_player);
};

