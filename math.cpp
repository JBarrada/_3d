#include "math.h"

double cos(double x) {
	if(x < 0.0) 
		x = -x;
	while(M_PI < x)
		x -= M_2_PI;
	return 1.0 - (x*x/2.0)*(1.0 - (x*x/12.0) * (1.0 - (x*x/30.0) * (1.0-x*x/56.0)));
}
 
double sin(double x) {
	return cos(x-M_PI_2);
}

double tan(double x) {
	return sin(x)/cos(x);
}

double atan(double x) {
    return M_PI_4*x - x*(dabs(x) - 1)*(0.2447 + 0.0663*dabs(x));
}

/*
double atan2(double y, double x) {
	if (x == 0.0) 
		return (y > 0.0) ? M_PI_2 : -M_PI_2;
	
	double u = atan(y / x);
	if( x < 0.0 ) {
		if( u > 0.0 )
			u -= M_PI;
		else
			u += M_PI;
	}
	return u;
}
*/

double atan2(double y, double x) {
	double coeff_1 = M_PI / 4.0;
	double coeff_2 = 3.0 * coeff_1;
	double abs_y = dabs(y);
	double angle;
	if (x >= 0.0) {
		double r = (x - abs_y) / (x + abs_y);
		angle = coeff_1 - coeff_1 * r;
	} else {
		double r = (x + abs_y) / (abs_y - x);
		angle = coeff_2 - coeff_1 * r;
	}
	return y < 0.0 ? -angle : angle;
}

double acos(double x) {
    double a=1.43+0.59*x; a=(a+(2+2*x)/a)/2;
    double b=1.65-1.41*x; b=(b+(2-2*x)/b)/2;
    double c=0.88-0.77*x; c=(c+(2-a)/c)/2;
    return (8*(c+(2-a)/c)-(b+(2-2*x)/b))/6;
}

double dabs(double x) {
	if (x < 0)
		return -1*x;
	else 
		return x;
}

int iabs(int x) {
	if (x < 0)
		return -1*x;
	else 
		return x;
}

double pow(double x, double e) {
	double value = 1;
	for (int i=0; i < e; i++) {
		value *= x;
	}
	return value;
}

double sqrt(const double x) { 
	double n = x / 2.0;
	double last_x = 0.0; 
	while(n != last_x) { 
		last_x = n;
		n = (n + x/n) / 2.0; 
	}
	return n;
}  

int round(double d) {
	if (d > 0)
		return (int)(d + 0.5);
	else
		return (int)(d - 0.5);
}

double interpolate(double start, double end, double pos) {
	return ((end-start)*pos) + start;
}

double constrain(double value, double min, double max) {
	return dmin(dmax(value, min), max);
}

double dmax(double a, double b) {
	if (a>b)
		return a;
	else
		return b;
}

double dmin(double a, double b) {
	if (a<b)
		return a;
	else
		return b;
}
