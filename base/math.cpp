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

double dabs(double x) {
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
