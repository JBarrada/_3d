#ifndef LEVEL_H
#define LEVEL_H

#include "gfx.h"
#include "vector.h"
#include "matrix.h"
#include "math.h"
#include "model.h"
#include <stdint.h>
#include <stdio.h>

/*
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
*/

class Surface {
public:
	int ab;
	int bc;
	int ca;
	
	void print() {printf("AB: %d BC: %d CA: %d\n", ab, bc, ca);};
};

class Level {
public:
	Model m;
	Surface* surfaces;
	
	Level();
	Level(Model m);
	
	int current_surface;
	Vector surface_pos;
	Vector world_pos;
	
	bool move(Vector& step);
	
	int find_neighbor(int p1, int p2, int avoid);
	void generate_level();
};

#endif