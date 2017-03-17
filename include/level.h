#ifndef LEVEL_H
#define LEVEL_H

#include "gfx.h"
#include "vector.h"
#include "matrix.h"
#include "math.h"
#include "model.h"
#include <stdint.h>
#include <stdio.h>

class Surface {
private:
	void generate_face();
public:
	Model m;
	
	Vector pos; // lower left
	Vector up;
	double width;
	double height;
	
	int u, d, l, r;
	
	Matrix surface_to_world;
	
	Surface();
	Surface(Vector, Vector, double, double);
	Surface(Vector, Vector, double, double, int, int, int, int);
	
	int can_move(const Vector&);
	Vector world_pos(const Vector&);
};

#endif