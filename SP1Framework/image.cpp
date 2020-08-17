#include "image.h"

image::image() :text(' '), colour(RGB(0,0,0)) {}
image::image(char text, WORD colour):text(text), colour(colour) {}
image::~image() {
	//do nothing
}

//getters
char image::gettext() {
	return text;
}
WORD image::getcolour() {
	return colour;
}

//setters
void image::settext(char c) {
	text = c;
}
void image::setcolour(WORD colour) {
	this->colour = colour;
}