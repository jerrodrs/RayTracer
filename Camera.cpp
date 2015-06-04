#include "Camera.h"

Camera::Camera() {
	camposition = Vect(0,0,0);
	camdirection = Vect(1,0,0);
	camright = Vect(0,0,0);
	camdown = Vect(0,0,0);
}

Camera::Camera(Vect pos, Vect dir, Vect right, Vect down) {
	camposition = pos;
	camdirection = dir;
	camright = right;
	camdown = down;
}