#ifndef THREED_H
#define THREED_H

#include "gfx.h"
#include "vector.h"
#include "matrix.h"
#include "math.h"
#include "model.h"
#include <stdint.h>
#include <stdio.h>

class ThreeD {
private:
	void tft(Vector, Vector, Vector, uint32_t);
	void bft(Vector, Vector, Vector, uint32_t);
public:
	Matrix p_matrix;
	Matrix v_matrix;
	Matrix pv_matrix;
	
	Vector camera_pos;
	Vector np_n;
	double np_d;
	
	double render_w, render_h;
	double depth_buffer[SCREEN_W*SCREEN_H];
	
	ThreeD();
	void init(Matrix, Matrix, double, double);
	
	void update_v_matrix(Matrix);
	void update_p_matrix(Matrix);
	
	Vector get_projected(Vector);
	
	bool on_screen(const Vector&);
	
	void draw_point(Vector, uint32_t);
	void draw_point_3d(Vector, uint8_t);
	
	void draw_line(Vector, Vector, uint32_t);
	void draw_line_3d(Vector, Vector, uint8_t);
	
	void draw_triangle(Vector, Vector, Vector, uint32_t);
	
	void draw_model_3d(const Model&, uint32_t);
	
	void clear_depth_buffer();
};

Matrix look_at_camera(Vector, Vector, Vector);

Matrix projection_ortho(double, double, double, double);
Matrix projection_persp(double, double, double, double);
Matrix projection_persp_gl(double, double, double, double);

Vector ray_plane_intersect(Vector&, Vector&, Vector&, double);
double point_plane_side(Vector&, Vector&, Vector&);
#endif