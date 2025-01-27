#include "matrix.h"

Matrix::Matrix() {
	this->w = 0;
	this->h = 0;
	memset(this->x, 0, sizeof(double)*16);
}

Matrix::Matrix(int w, int h) {
	this->w = w;
	this->h = h;
	memset(this->x, 0, sizeof(double)*16);
}

Matrix::Matrix(int w, int h, double *x) {
	this->w = w;
	this->h = h;
	memcpy(this->x, x, sizeof(double)*16);
}

Matrix::Matrix(Vector v) {
	this->w = 1;
	this->h = 4;
	double m_x[] = {v.x, v.y, v.z, 1, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	memcpy(this->x, m_x, sizeof(double)*16);
}

void Matrix::print() {
	for (int h=0; h<this->h; h++) {
		for (int w=0; w<this->w; w++) {
			printf("%4.1f ", this->x[(h*this->w) + w]);
		}
		printf("\n");
	}
}

Matrix Matrix::operator*(const Matrix& b) {
	Matrix c(b.w, this->h);
	
	for (int i=0; i<this->h; i++)
		for (int j=0; j<b.w; j++)
			for (int k=0; k<b.h; k++)
				c.x[i*c.w+j] += this->x[i*this->w+k]*b.x[k*b.w+j];
	
	return c;
}

Matrix Matrix::operator*=(const Matrix& b) {
	double o[16];
	memcpy(o, this->x, sizeof(double)*16);
	int o_w = this->w, o_h = this->h;
	
	this->w = b.w;
	
	for (int i=0; i<o_h; i++)
		for (int j=0; j<b.w; j++)
			for (int k=0; k<b.h; k++)
				this->x[i*b.w+j] += o[i*o_w+k]*b.x[k*b.w+j];
	
	return *this;
}

Vector Matrix::get_vector() {
	return (Vector) {this->x[0], this->x[1], this->x[2]};
}

void Matrix::rotate_3d(Vector v, double angle) {
	double s = sin(angle);
	double c = cos(angle);
	
	v = v.norm();
	Vector v2(v.x*v.x, v.y*v.y, v.z*v.z);
	
	double m_x[] = {c+v2.x*(1-c), v.x*v.y*(1-c)-v.z*s, v.x*v.z*(1-c)+v.y*s,
					v.y*v.x*(1-c)+v.z*s, c+v2.y*(1-c), v.y*v.z*(1-c)-v.x*s,
					v.z*v.x*(1-c)-v.y*s, v.z*v.y*(1-c)+v.x*s, c+v2.z*(1-c)};
	Matrix m(4, 4, m_x);
	
	*this *= m;
}

void Matrix::rotate_3d_x(double value) {
	double s = sin(value);
	double c = cos(value);
	double m_x[] = {1, 0, 0, 0,  0, c, -s, 0,  0, s, c, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	*this *= m;
}
Matrix Matrix::rotated_3d_x(double value) {
	double s = sin(value);
	double c = cos(value);
	double m_x[] = {1, 0, 0, 0,  0, c, -s, 0,  0, s, c, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	return *this * m;
}

void Matrix::rotate_3d_y(double value) {
	double s = sin(value);
	double c = cos(value);
	double m_x[] = {c, 0, s, 0,  0, 1, 0, 0,  -s, 0, c, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	*this *= m;
}
Matrix Matrix::rotated_3d_y(double value) {
	double s = sin(value);
	double c = cos(value);
	double m_x[] = {c, 0, s, 0,  0, 1, 0, 0,  -s, 0, c, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	return *this * m;
}

void Matrix::rotate_3d_z(double value) {
	double s = sin(value);
	double c = cos(value);
	double m_x[] = {c, -s, 0, 0,  s, c, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	*this *= m;
}
Matrix Matrix::rotated_3d_z(double value) {
	double s = sin(value);
	double c = cos(value);
	double m_x[] = {c, -s, 0, 0,  s, c, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	return *this * m;
}

void Matrix::scale(double x, double y, double z) {
	double m_x[] = {x, 0, 0, 0,  0, y, 0, 0,  0, 0, z, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	*this *= m;
}
void Matrix::scale(Vector v) {
	Matrix::scale(v.x, v.y, v.z);
}
Matrix Matrix::scaled(double x, double y, double z) {
	double m_x[] = {x, 0, 0, 0,  0, y, 0, 0,  0, 0, z, 0,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	return *this * m;
}
Matrix Matrix::scaled(Vector v) {
	return Matrix::scaled(v.x, v.y, v.z);
}

void Matrix::translate(double x, double y, double z) {
	double m_x[] = {1, 0, 0, x,  0, 1, 0, y,  0, 0, 1, z,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	*this *= m;
}
void Matrix::translate(Vector v) {
	Matrix::translate(v.x, v.y, v.z);
}

Matrix Matrix::translated(double x, double y, double z) {
	double m_x[] = {1, 0, 0, x,  0, 1, 0, y,  0, 0, 1, z,  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	
	return *this * m;
}
Matrix Matrix::translated(Vector v) {
	return Matrix::translated(v.x, v.y, v.z);
}