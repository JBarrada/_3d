#ifndef MATH_H
#define MATH_H

#define M_PI 3.14159265
#define M_PI_2 1.57079632
#define M_PI_4 0.78539816
#define M_2_PI 6.28318530

double cos(double x);
double sin(double x);
double tan(double x);
double atan(double x);
double atan2(double y, double x);
double acos(double x);

double dabs(double x);
int iabs(int x);
double pow(double x, double e);
double sqrt(const double x);
int round(double d);
double interpolate(double start, double end, double pos);

double constrain(double value, double min, double max);
double dmax(double, double);
double dmin(double, double);

#endif