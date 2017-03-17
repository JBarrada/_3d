#ifndef MATRIX_H
#define MATRIX_H

#include "math.h"
#include "vector.h"
#include <string.h>
#include <stdio.h>


class Matrix {
public:
	int h, w;
	double x[16];
	
	Matrix();
	Matrix(int, int);
	Matrix(int, int, double*);
	Matrix(Vector);
	
	Matrix operator*(const Matrix&);
	Matrix operator*=(const Matrix&);
	
	Vector get_vector();
	
	void rotate_3d(Vector, double);
	void rotate_3d(Vector v, double, double);
	Matrix rotated_3d(Vector, double);
	Matrix rotated_3d(Vector, double, double);
	
	void rotate_3d_x(double);
	void rotate_3d_y(double);
	void rotate_3d_z(double);
	
	Matrix rotated_3d_x(double);
	Matrix rotated_3d_y(double);
	Matrix rotated_3d_z(double);
	
	
	void scale(double, double, double);
	void scale(Vector);
	
	Matrix scaled(double, double, double);
	Matrix scaled(Vector);
	
	
	void translate(double, double, double);
	void translate(Vector);
	
	Matrix translated(double, double, double);
	Matrix translated(Vector);
	
	void print();
};

extern Matrix IDENTITY;

#endif