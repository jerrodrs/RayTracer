#include "Color.h"

Color::Color () {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double x, double y, double z, double s) {
	red = x;
	green = y;
	blue = z;
	special = s;
}