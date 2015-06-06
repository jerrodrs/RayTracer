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

	double brightness(){
		return(red + green + blue)/3;
	}

	Color colorScalar(double scalar){
		return Color (red*scalar, green*scalar, blue*scalar, special);
	}

	Color colorAdd(Color color){
		return Color (red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), special);
	}

	Color colorMultiply(Color color){
		return Color (red*color.getColorRed(), green*color.getColorGreen(), blue*color.getColorBlue(), special);
	}

	Color colorAverage(Color color){
		return Color ((red + color.getColorRed())/2, (green + color.getColorGreen())/2, (blue + color.getColorBlue())/2, special);
	}

	Color clip();

};
#endif