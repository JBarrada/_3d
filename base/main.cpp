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

Vector camera_position(15,15,1);
double angle = 0;
double radius = 15.0;

Model test_model;
Model test_cube;

void idle() {
	//render();
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
	
	
	if (key == 'o') {
		radius += 0.2;
	}
	if (key == 'p') {
		radius -= 0.2;
	}
	
	camera_position.x = (cos(angle) * radius);
	camera_position.y = (sin(angle) * radius);
	
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	threed.update_v_matrix(v_matrix);

	render();
}

void draw() {
	threed.clear_depth_buffer();
	
	//threed.draw_model_3d(test_model, get_byte_color(255, 0, 0));
	threed.draw_model_3d(test_cube, get_byte_color(255, 255, 255));
	
	shade(threed.depth_buffer);
	toon(threed.depth_buffer);
}

void load_model(char* file, Model* model) {
	std::ifstream ifs(file, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();
	int length = pos;
	
	char *data = new char[length];
	ifs.seekg(0, std::ios::beg);
	ifs.read(data, length);
	
	ifs.close();
	
	model->load_data(data, length);
}

int main() {
	//load_model("test.obj", &test_model);
	//load_model("cube.obj", &test_cube);
	
	//test_model.transform = test_model.transform.rotated_3d_z(1.5);
	//test_model.transform = test_model.transform.translated(10, 0, 0);
	
	//test_cube = create_face((Vector){0,1,0},(Vector){1,1,0},(Vector){1,0,0},(Vector){0,0,0});
	//test_model = create_face((Vector){0,1,1},(Vector){1,1,1},(Vector){1,1,0},(Vector){0,1,0});
	
	test_cube = create_box((Vector){0,0,0}, 1, 2, 3);
	//test_cube.print();
	
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

