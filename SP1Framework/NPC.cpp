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
void NPC::update(map& g_map, map& bg_map, map& bgc_map, map& fg_map) {
	if (currentsentenceposition <= dialogue.length())
	{
		char char_to_add = dialogue[currentsentenceposition];
		if (char_to_add == '`')
		{
			currentsentencenumber += 1;
			displaytext(currentsentences, currentsentencenumber, FOREGROUND_RED + BACKGROUND_GREEN, fg_map);
			int lengthy = currentsentences[currentsentencenumber - 2].length();
			for (int i = 0; i < lengthy; i++)
			{
				fg_map.clearmap();
				fg_map.setmapposition(position(pos.get('x') - lengthy + i, pos.get('y') - (currentsentencenumber - 2)), image(NULL, 0));
			}
			
		}
		else
		{
			currentsentences[currentsentencenumber - 1] += char_to_add;
			displaytext(currentsentences, currentsentencenumber, FOREGROUND_RED + BACKGROUND_GREEN, fg_map);
		}
		currentsentenceposition += 1;
	}
}