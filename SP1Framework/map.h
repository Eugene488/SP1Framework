#pragma once
#include "image.h"
#include "position.h"
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <sstream>

class map
{
private:
	//camera members
	position camerapos; //camera will be 0,0 for the rendering
	position camerasize; //camera size is how big the camera is (square shaped); while it uses position, it's just to store the xy size

	//map members
	image** mapvalues;
	int size_x;
	int size_y;
public:
	map(int size_x, int size_y);
	map(int size_x, int size_y, position camerapos, position camera_size); //CAMERA SIZE CAN ONLY BE ODD
	~map();

	//getters
	//camera
	position getcampos();
	position getcamsize();

	//map
	image** getmap();
	image getmapposition(position);
	int getmapsize(char xy);

	//setters
	//map
	void setmapposition(position, image);
	void setmapposition(position start_pos, string& string, WORD colour);

	//other methods
	//map
	void clearmap();
	void map::fill(image* image_arr, int image_arr_size, int* weightage);
	//camera
	void centerOnPlayer(position playerpos);
};

