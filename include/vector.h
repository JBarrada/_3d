#ifndef VECTOR_H
#define VECTOR_H

#include "math.h"

class Vector {
public:
	double x, y, z;
	
	Vector();
	Vector(double, double, double);
	
	Vector operator+(const Vector&);
	Vector operator+=(const Vector&);
	Vector operator-(const Vector&);
	Vector operator-=(const Vector&);
	Vector operator*(double);
	Vector operator*=(double);
	Vector operator/(double);
	Vector operator/=(double);
	bool operator==(const Vector&);
	
	double dot(const Vector&);
	Vector cross(const Vector&);
	Vector project(Vector&);
	Vector reject(Vector&);
	double length();
	
	Vector norm();
	
	void print();
};

void swap(Vector*, Vector*);
Vector calculate_surface_normal(Vector&, Vector&, Vector&);

Vector intersect(Vector& pa1, Vector& pb1, Vector& pa2, Vector& pb2);

#endif