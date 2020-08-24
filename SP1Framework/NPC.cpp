#include "NPC.h"
NPC::NPC(position position, int hp, float spd, image image, string texts, string name, map& g_map) :entity(position, image, spd, hp, name), dialogue(texts) {
	setpos(pos, g_map);
}
NPC::~NPC() {
	//do nothing
}

//other methods
void NPC::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	//TODO
}
void NPC::displaytext(string* sentence, int lines, WORD textcolour, map& fg_map) {
	for (int i = 0; i < lines; i++)
	{
		fg_map.setmapposition(position(pos.get('x') - sentence[i].length() / 2, pos.get('y') - (lines - i)), sentence[i], textcolour);
	}
}
void NPC::update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* g_player) {
	float distancefromplayer = sqrt(pow(pos.get('x') - g_player->getpos().get('x'), 2) + pow(pos.get('y') - g_player->getpos().get('y'),2));
	extern float debugtext;
	debugtext = distancefromplayer;
	if (distancefromplayer <= 15)
	{
		if (currentsentenceposition <= dialogue.length())
		{
			char char_to_add = dialogue[currentsentenceposition];
			if (char_to_add == '`')
			{
				currentsentencenumber += 1;
				displaytext(currentsentences, currentsentencenumber, 15, fg_map);
				int lengthy = currentsentences[currentsentencenumber - 2].length();
				for (int i = 0; i < lengthy; i++)
				{
					fg_map.setmapposition(position(pos.get('x') - lengthy/2 + i, pos.get('y') - 1), image(NULL, 0));
				}

			}
			else
			{
				currentsentences[currentsentencenumber - 1] += char_to_add;
				displaytext(currentsentences, currentsentencenumber, 15, fg_map);
			}
			currentsentenceposition += 1;
		}
	}
	else
	{
		if (currentsentences[0] == "" && currentsentences[1] == "" && currentsentences[2] == "")
		{
			//do nothing
		}
		else
		{
			//removing the text on screen
			for (int i = 0; i < currentsentencenumber; i++)
			{
				int lengthy = currentsentences[i].length();
				for (int i2 = 0; i2 < lengthy; i2++)
				{
					fg_map.setmapposition(position(pos.get('x') - lengthy/2 + i2, pos.get('y') - (currentsentencenumber - i)), image(NULL, 0));
				}
				currentsentences[i] = "";
			}
			currentsentencenumber = 1;
			currentsentenceposition = 0;
		}
	}
}