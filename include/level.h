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
public:
	Model m;
	
	Vector pos; // lower left
	Vector up;
	double width;
	double height;
	
	int u, d, l, r;
	
	
	Surface();
	
	
}

#endif