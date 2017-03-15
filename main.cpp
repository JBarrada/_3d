#include "matrix.h"
#include "vector.h"
#include "math.h"
#include "threed.h"
#include "gfx.h"
#include "model.h"

#include <stdio.h>

#include <iostream>
#include <fstream>

ThreeD threed;

Vector camera_position(10,10,1);
double angle = 0;

Model test_model;

void idle() {
	
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') {
		camera_position += (Vector){0, 0, 0.2};
	}
	if (key == 's') {
		camera_position += (Vector){0, 0, -0.2};
	}
	
	if (key == 'a') {
		angle += 0.1;
	}
	if (key == 'd') {
		angle -= 0.1;
	}
	
	camera_position.x = (cos(angle) * 10.0);
	camera_position.y = (sin(angle) * 10.0);
	
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	threed.update_v_matrix(v_matrix);

	render();
}

void draw() {
	threed.clear_depth_buffer();
	
	/*
	threed.draw_point_3d((Vector){-0.5,-0.5,-0.5},20);
	threed.draw_point_3d((Vector){-0.5, 0.5,-0.5},20);
	threed.draw_point_3d((Vector){ 0.5, 0.5,-0.5},20);
	threed.draw_point_3d((Vector){ 0.5,-0.5,-0.5},20);
	
	threed.draw_point_3d((Vector){-0.5,-0.5,0.5},50);
	threed.draw_point_3d((Vector){-0.5,0.5,0.5},50);
	threed.draw_point_3d((Vector){0.5,0.5,0.5},50);
	threed.draw_point_3d((Vector){0.5,-0.5,0.5},50);
	
	threed.draw_line_3d((Vector){-0.5,-0.5,-0.5}, (Vector){-0.5, 0.5,-0.5}, 20);
	threed.draw_line_3d((Vector){-0.5, 0.5,-0.5}, (Vector){ 0.5, 0.5,-0.5}, 20);
	threed.draw_line_3d((Vector){ 0.5, 0.5,-0.5}, (Vector){ 0.5,-0.5,-0.5}, 20);
	threed.draw_line_3d((Vector){ 0.5,-0.5,-0.5}, (Vector){-0.5,-0.5,-0.5}, 20);
	*/
	
	threed.draw_model_3d(test_model, 90);
}

void load_model() {
	std::ifstream ifs("test.obj", std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();
	int length = pos;
	
	char *data = new char[length];
	ifs.seekg(0, std::ios::beg);
	ifs.read(data, length);
	
	ifs.close();
	
	test_model.load_data(data, length);
}

int main() {
	load_model();
	
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

