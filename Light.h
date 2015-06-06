#ifndef _LIGHT_H
#define _LIGHT_H

#include "Source.h"
#include "Vect.h"
#include "Color.h"

class Light : public Source{
	Vect position;
	Color color;

public:
	Light ();

	Light (Vect, Color);

	//methods

	Vect getLightPosition() {return position;}
	Color getColor() {return color;}


};

#endif