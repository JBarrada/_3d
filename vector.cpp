#include "vector.h"
#include <stdio.h>


Vector::Vector() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector::Vector(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector Vector::operator+(const Vector& b) {
	Vector out(this->x+b.x, this->y+b.y, this->z+b.z);
	return out;
}

Vector Vector::operator+=(const Vector& b) {
	this->x += b.x;
	this->y += b.y;
	this->z += b.z;
	return *this;
}

Vector Vector::operator-(const Vector& b) {
	Vector out(this->x-b.x, this->y-b.y, this->z-b.z);
	return out;
}

Vector Vector::operator-=(const Vector& b) {
	this->x -= b.x;
	this->y -= b.y;
	this->z -= b.z;
	return *this;
}

Vector Vector::operator*(double d) {
	Vector out(this->x*d, this->y*d, this->z*d);
	return out;
}
Vector Vector::operator*=(double d) {
	this->x *= d;
	this->y *= d;
	this->z *= d;
	
	return *this;
}

Vector Vector::operator/(double d) {
	Vector out(this->x/d, this->y/d, this->z/d);
	return out;
}
Vector Vector::operator/=(double d) {
	this->x /= d;
	this->y /= d;
	this->z /= d;
	
	return *this;
}

bool Vector::operator==(const Vector& b) {
	return ((this->x == b.x) && (this->y == b.y) && (this->z == b.z));
}

double Vector::dot(const Vector& b) {
	return this->x * b.x + this->y * b.y + this->z * b.z;
}

Vector Vector::cross(const Vector& b) {
	return (Vector){this->y*b.z - b.y*this->z, this->z*b.x - b.z*this->x, this->x*b.y - b.x*this->y};
}

Vector Vector::project(Vector& b) {
	Vector a = *this;
	return b * (b.dot(a)) / (b.dot(b));
}

Vector Vector::reject(Vector& b) {
	Vector a = *this;
	return a - a.project(b);
}

double Vector::length() {
	return sqrt((this->x*this->x)+(this->y*this->y)+(this->z*this->z));
}

Vector Vector::norm() {
	Vector v = *this;
	return (v / this->length());
}

void Vector::print() {
	printf("%5.2f, %5.2f, %5.2f\n", this->x, this->y, this->z);
}

void swap(Vector* a, Vector* b) {
	Vector temp = *a;
	*a = *b;
	*b = temp;
}

Vector calculate_surface_normal(Vector& a, Vector& b, Vector& c) {
	Vector u = (b - a);
	Vector v = (c - a);
	
	Vector normal;
	normal.x = (u.y*v.z) - (u.z*v.y);
	normal.y = (u.z*v.x) - (u.x*v.z);
	normal.z = (u.x*v.y) - (u.y*v.x);
	
	double comp_sum = dabs(normal.x) + dabs(normal.y) + dabs(normal.z);
	normal.x /= comp_sum;
	normal.y /= comp_sum;
	normal.z /= comp_sum;
	
	return normal;
}

Vector intersect(Vector& pa1, Vector& pb1, Vector& pa2, Vector& pb2) {
	double a1 = pb1.y - pa1.y;
	double b1 = pa1.x - pb1.x;
	double c1 = a1*pa1.x + b1*pa1.y;
	
	double a2 = pb2.y - pa2.y;
	double b2 = pa2.x - pb2.x;
	double c2 = a2*pa2.x + b2*pa2.y;
	
	double det = a1*b2 - a2*b1;
	
	if (det == 0) {
		return (Vector){0,0,0}; // parallel
	} else {
		return (Vector){(b2*c1 - b1*c2) / det, (a1*c2 - a2*c1) / det, 0};
	}
}
