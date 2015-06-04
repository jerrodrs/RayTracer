#include "Plane.h"


Plane::Plane() {
	normal = Vect(1,0,0);
	distance = 0.0;
	color = Color(0.5,0.5,0.5,0);
}

Plane::Plane(Vect norm, double dist, Color c) {
	normal = norm;
	distance = dist;
	color = c;
}

double Plane::findIntersection(Ray ray){
	Vect ray_direction = ray.getRayDirection();

	double a = ray_direction.dotProduct(normal);

	if (a == 0){
		//ray is parallel to the plan
		return -1;
	}else{
		double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
		return -1*b/a;
	}

}