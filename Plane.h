#ifndef _PLANE_H
#define _PLANE_H

#include "Vect.h"
#include "Color.h"
#include "math.h"
#include "Object.h"

class Plane : public Object{
	Vect normal;
	double distance;
	Color color;

public:
	Plane ();

	Plane (Vect, double, Color);

	//methods

	Vect getPlaneNormal() {return normal;}
	double getPlaneDistance() {return distance;}
	Color getPlaneColor() {return color;}

	Vect getNormalAt (Vect point){
		return normal;
	}

	double findIntersection(Ray);




};

#endif