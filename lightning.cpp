#include "lightning.h"

lightning::lightning(position pos, map& fg_map): entity(pos, image(NULL, 0), 1, 1, "") {
    for (int i = 0; i < 25; i++)
    {
        fg_map.setmapposition(position(pos.get('x'), pos.get('y') - i), image('V', 11 + 240));
    }
}
lightning::~lightning() {
	//do nothing
}

void lightning::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
    for (int i = 0; i < 25; i++)
    {
        extern map fg_map;
        fg_map.setmapposition(position(pos.get('x'), pos.get('y') - i), image(NULL, 0));
    }
    for (int i = 0; i < MAXENTITY; i++)
    {
        if (entities[i] == NULL)
        {
            entities[i] = new fire(pos, 3, 3, bgc_map, bg_map);
            break;
        }
    }
    hp = 0;
}

void lightning::die(map& g_map, map& bg_map, map& bgc_map) {
    //do nothing
}