#pragma once
#include "entity.h"
class fire : public entity
{
private:
    const static WORD states_bg_colours[3];
    //int state; will be hp instead
public:
    fire(float spd, map& g_map, map& bg_map, map& bgc_map);
    fire(position pos, float spd, int hp, map& gmap, map& bgmap);
    ~fire();

    //other methods
    void move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY);
    void die(map& g_map, map& bg_map, map& bgc_map);
    void update(map& g_map, map& bg_map, map& bgc_map, map& fg_map, entity* player);
};

