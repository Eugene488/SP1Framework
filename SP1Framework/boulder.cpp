#include "boulder.h"

boulder::boulder(position position, float spd, map& g_map) : entity(position, image(35, 8), spd, 5, "barricade") {
	setpos(pos, g_map);
}
boulder::~boulder() {
	//do nothing
}


//other methods

void boulder::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	pushtimer += 0.2f;
	if (pushtimer >= pushtime)
	{
		pushtimer = 0;
		position futurloc = pos;
		if (entities[0]->getpos().get('x') < pos.get('x'))
		{
			futurloc.set('x', futurloc.get('x') + 1);
		}
		else if (entities[0]->getpos().get('x') > pos.get('x'))
		{
			futurloc.set('x', futurloc.get('x') - 1);
		}
		if (entities[0]->getpos().get('y') > pos.get('y'))
		{
			futurloc.set('y', futurloc.get('y') - 1);
		}
		else if (entities[0]->getpos().get('y') < pos.get('y'))
		{
			futurloc.set('y', futurloc.get('y') + 1);
		}
		if (collisiondetection(solids, listsize, futurloc, g_map))
		{
			prevloc = getpos();
			g_map.setmapposition(prevloc, previmg);
			previmg = image(NULL, 0);
			setpos(futurloc, g_map);
		}
	}
}
void boulder::updatetimers(float dt) {
	//do nothing
}