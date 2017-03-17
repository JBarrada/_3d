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

double Vector::dot(const Vector& b) {
	return this->x * b.x + this->y * b.y + this->z * b.z;
}

Vector Vector::cross(const Vector& b) {
	return (Vector){this->y*b.z - b.y*this->z, this->z*b.x - b.z*this->x, this->x*b.y - b.x*this->y};
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

