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

Color Color::clip(){
	double alllight = red + blue + green;
	double excesslight = alllight - 3;
	if (excesslight > 0){
		red = red + excesslight*(red/alllight);
		green = green + excesslight*(green/alllight);
		blue = blue + excesslight*(blue/alllight);
	}
	if(red > 1) {red = 1;}
	if(green > 1) {green = 1;}
	if(blue > 1) {blue = 1;}
	if(red < 0) {red = 0;}
	if(green < 0) {green = 0;}
	if(blue < 0) {blue = 0;}

	return Color (red, green, blue, special);
}