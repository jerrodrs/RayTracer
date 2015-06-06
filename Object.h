#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object{

public:
	Object ();

	//methods
	virtual Color getColor() {return Color (0.0, 0.0, 0.0, 0);}

	virtual double findIntersection(Ray);

	virtual Vect getNormalAt (Vect point){
		return Vect (0,0,0);
	}

};

#endif