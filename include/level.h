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
	
	uint32_t color;
	
	int u, d, l, r;
	
	Matrix surface_to_world;
	
	Surface();
	Surface(Vector, Vector, double, double, uint32_t);
	Surface(Vector, Vector, double, double, int, int, int, int, uint32_t);
	
	int can_move(const Vector&);
	Vector world_pos(const Vector&);
};

class Level {
public:
	Surface *surfaces;
	int surfaces_count;
	
	int current_surface;
	Vector surface_position;
	Vector world_position;
	
	Level();	
	bool move(Vector);
};

#endif