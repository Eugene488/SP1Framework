#include "projectile.h"

projectile::projectile(position startpos, position endpos, image img, float spd, string name, map& g_map, string target, int dmg) : entity(startpos, img, spd, 1, name), truex(startpos.get('x')), truey(startpos.get('y')), target(target), dmg(dmg){
	float distance = sqrt(pow(endpos.get('x') - startpos.get('x'), 2) + pow(endpos.get('y') - startpos.get('y'), 2));
	direction.set('x', (endpos.get('x') - startpos.get('x'))/distance);
	direction.set('y', (endpos.get('y') - startpos.get('y')) / distance);
	g_map.setmapposition(pos, img);
}
projectile::~projectile() {
	//do nothing
}

//other methods
void projectile::move(map& g_map, map& bg_map, map& bgc_map, WORD solids[], int listsize, entity** entities, int MAXENTITY) {
	truex += direction.get('x');
	truey += direction.get('y');
	position futurloc = position(truex, truey);

	if (collisiondetection(solids, listsize, futurloc, g_map))
	{
		prevloc = getpos();
		g_map.setmapposition(prevloc, previmg);
		previmg = image(NULL, 0);
		setpos(futurloc, g_map);
		if (triggerdetection(g_map, bg_map, bgc_map, pos, "g_map") == target)
		{
			extern void getentityfrompos(int* ptr, position pos, map & g_map);
			int* idxs = new int[MAXENTITY];
			getentityfrompos(idxs, pos, g_map);
			if (idxs[0] != -1)
			{
				for (int i = 0; i < MAXENTITY; i++)
				{
					if (idxs[i] == NULL)
					{
						break;
					}
					else if (entities[idxs[i]]->getname() == target)
					{
						entities[idxs[i]]->sethp(entities[idxs[i]]->gethp() - dmg);
					}
				}
			}
		}
	}
	else
	{
		die(g_map, bg_map, bgc_map);
	}
}