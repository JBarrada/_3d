#ifndef MODEL_H
#define MODEL_H

#include "vector.h"
#include "matrix.h"
#include "math.h"
#include "string_tools.h"
#include <stdint.h>
#include <stdio.h>


struct Triangle {
	int a, b, c, normal;
};

class Model {
public:
	
	Model();
	Model(char*, int);
	
	void load_data(char*, int);

	int points_count;
	int normals_count;
	int triangles_count;
	
	Vector *points;
	Vector *normals;
	Triangle *triangles;
	
	Matrix transform;
};

#endif