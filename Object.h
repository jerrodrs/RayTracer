#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object{

public:
	Object ();

	//methods
	Color getObjectColor() {return Color (0.0, 0.0, 0.0, 0);}

	double findIntersection(Ray);


};

#endif