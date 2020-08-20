#include "map.h"


map::map(int size_x, int size_y):size_x(size_x), size_y(size_y) {
	mapvalues = new image*[size_x];
	//malloc
	for (int x = 0; x < size_x; x++)
	{
		mapvalues[x] = new image[size_y];
	}
	//init
	for (int x = 0; x < size_x; x++)
	{
		for (int y = 0; y < size_y; y++)
		{
			mapvalues[x][y] = image();
		}
	}
}

map::map(int size_x, int size_y, position camerapos, position camerasize) :size_x(size_x), size_y(size_y), camerapos(camerapos), camerasize(camerasize) {
	mapvalues = new image * [size_x];
	//malloc
	for (int x = 0; x < size_x; x++)
	{
		mapvalues[x] = new image[size_y];
	}
	//init
	for (int x = 0; x < size_x; x++)
	{
		for (int y = 0; y < size_y; y++)
		{
			mapvalues[x][y] = image(NULL,0);
		}
	}
}

map::~map() {
	delete mapvalues;
}

//getters
//camera
position map::getcampos() {
	return camerapos;
}
position map::getcamsize() {
	return camerasize;
}

//map
image** map::getmap(){
	return mapvalues;
}
image map::getmapposition(position pos) {
	return mapvalues[pos.get('x')][pos.get('y')];
}
int map::getmapsize(char xy) {
	if (xy == 'x')
	{
		return size_x;
	}
	else if (xy == 'y')
	{
		return size_y;
	}
	return NULL;
}

//setters
//map
void map::setmapposition(position pos, image i) {
	mapvalues[pos.get('x')][pos.get('y')] = i;
}

//other methods
//map
void map::clearmap() {
	for (int x = 0; x < size_x; x++)
	{
		for (int y = 0; y < size_y; y++)
		{
			mapvalues[x][y] = image();
		}
	}
}

void map::fill(image* images, int size, int* weightage) {
	int wsum = 0;
	int indx = 0;
	int randnum = 0;
	for (int i = 0; i < size; i++)
	{
		wsum += weightage[i];
	}
	for (int x = 0; x < size_x; x++)
	{
		for (int y = 0; y < size_y; y++)
		{
			indx = 0;
			randnum = rand() % wsum;
			while (randnum >= weightage[indx]) {
				randnum -= weightage[indx];
				indx++;
			}
			mapvalues[x][y] = image(images[indx]);
		}
	}
}

//camera
void map::centerOnPlayer(position playerpos) {
	camerapos = position((playerpos.get('x') - ((camerasize.get('x')-1) / 2)), (playerpos.get('y') - ((camerasize.get('y')-1) / 2)));
}