#ifndef _RAY_H
#define _RAY_H
#include "Vect.h"

class Ray{
	Vect origin, direction;

public:
	Ray ();

	Ray (Vect, Vect);

	//methods

	Vect getRayOrigin() {return origin;}
	Vect getRayDirection() {return direction;}


};

#endif