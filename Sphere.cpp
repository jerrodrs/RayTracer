#include "Sphere.h"


Sphere::Sphere() {
	center = Vect(0,0,0);
	radius = 1.0;
	color = Color(0.5,0.5,0.5,0);
}

Sphere::Sphere(Vect cent, double r, Color c) {
	center = cent;
	radius = r;
	color = c;
}

double Sphere::findIntersection(Ray ray){
	Vect ray_origin = ray.getRayOrigin();
	double ray_origin_x = ray_origin.getVectX();
	double ray_origin_y = ray_origin.getVectY();
	double ray_origin_z = ray_origin.getVectZ();

	Vect ray_direction = ray.getRayDirection();
	double ray_direction_x = ray_direction.getVectX();
	double ray_direction_y = ray_direction.getVectY();
	double ray_direction_z = ray_direction.getVectZ();

	Vect sphere_center = center;
	double sphere_center_x = sphere_center.getVectX();
	double sphere_center_y = sphere_center.getVectY();
	double sphere_center_z = sphere_center.getVectZ();

	double a = 1; //normalized
	double b = (2*(ray_origin_x - sphere_center_x)*ray_direction_x) + (2*(ray_origin_y - sphere_center_y)*ray_direction_y) + (2*(ray_origin_z - sphere_center_z)*ray_direction_z);
	double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius*radius);

	double discriminant = b*b - 4*c;

	if (discriminant > 0){
		//the ray intersects the sphere
		//can intersect on two sides, two roots

		//first root
		double root_1 = ((-1*b - sqrt(discriminant))/2) - 0.000001;

		if (root_1 > 0){
			//the first root is the smallest positive root
			return root_1;
		}else{
			//the second root is the smallest positive root
			double root_2 = sqrt((sqrt(discriminant) - b)/2) - 0.000001;
			return root_2;
		}
	}else{
		//the ray missed the sphere
		return -1;
	}

}

Vect Sphere::getNormalAt(Vect point){
	Vect normal_vect = point.vectAdd(center.negative()).normalize();
	return normal_vect;
}