#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vect.h"
#include "Color.h"

class Light{
	Vect position;
	Color color;

public:
	Light ();

	Light (Vect, Color);

	//methods

	virtual Vect getLightPosition() {return position;}
	virtual Color getLightColor() {return color;}


};

#endif