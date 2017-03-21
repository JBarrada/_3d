#ifndef MODEL_H
#define MODEL_H

#include "vector.h"
#include "matrix.h"
#include "math.h"
#include "string_tools.h"
#include <stdint.h>
#include <stdio.h>


struct Triangle {
	int a, b, c, normal, mtl;
};

struct Material {
	char* name;
	uint32_t color;
};

class Model {
public:
	
	Model();
	Model(char*, int);
	
	void load_data(char*, int);

	void print();
	
	int get_material(char*);
	
	int points_count;
	int normals_count;
	int triangles_count; 
	
	Vector *points;
	Vector *normals;
	Triangle *triangles;
	
	int materials_count;
	Material *materials;
	
	Matrix transform;
};

Model create_face(Vector a, Vector b, Vector c, Vector d, uint32_t color);
Model create_face(Vector pos, double width, double height, Vector up, uint32_t color);
Model create_box(Vector c, double lx, double ly, double lz, uint32_t color);

#endif