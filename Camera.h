#ifndef _CAMERA_H
#define _CAMERA_H
#include "Vect.h"

class Camera{
	Vect camposition, camdirection, camright, camdown;

public:
	Camera ();

	Camera (Vect, Vect, Vect, Vect);

	//methods

	Vect getCameraPosition() {return camposition;}
	Vect getCameraDirection() {return camdirection;}
	Vect getCameraRight() {return camright;}
	Vect getCameraDown() {return camdown;}


};
#endif