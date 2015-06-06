#ifndef _SPHERE_H
#define _SPHERE_H

#include "Vect.h"
#include "Color.h"
#include "math.h"
#include "Object.h"

class Sphere : public Object{
	Vect center;
	double radius;
	Color color;

public:
	Sphere ();

	Sphere (Vect, double, Color);

	//methods

	Vect getSphereCenter() {return center;}
	double getSphereRadius() {return radius;}
	Color getColor() {return color;}

	Vect getNormalAt(Vect);

	double findIntersection(Ray);


};

#endif