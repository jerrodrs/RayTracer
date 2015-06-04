#ifndef _COLOR_H
#define _COLOR_H

#include "math.h"

class Color{
	double red, green, blue, special;

public:
	Color ();

	Color (double, double, double, double);

	//methods

	double getColorRed(){ return red;}
	double getColorGreen(){ return green;}
	double getColorBlue(){ return blue;}
	double getColorSpecial(){ return special;}

	double setColorRed(double redValue) {red = redValue;};
	double setColorGreen(double greenValue) {green = greenValue;};
	double setColorBlue(double blueValue) {blue = blueValue;};
	double setColorSpecial(double specialValue) {special = specialValue;};

};
#endif