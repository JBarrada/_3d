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

Vector player_position(0, 0, 0);
Vector player_up(0, 0, 1);
double player_direction = 0;
Vector camera_position(15,15,1);
double camera_follow = 15.0;
double camera_height = 3.0;

Vector d_camera_position(15,15,1);
Vector d_player_up(0, 0, 1);

double smooth_speed = 10.0;

Model player;
Model test_cube;

void idle() {
	d_camera_position.x += (camera_position.x - d_camera_position.x) / smooth_speed;
	d_camera_position.y += (camera_position.y - d_camera_position.y) / smooth_speed;
	d_camera_position.z += (camera_position.z - d_camera_position.z) / smooth_speed;
	

	d_player_up.x += (player_up.x - d_player_up.x) / smooth_speed;
	d_player_up.y += (player_up.y - d_player_up.y) / smooth_speed;
	d_player_up.z += (player_up.z - d_player_up.z) / smooth_speed;
	
	
	Matrix v_matrix = look_at_camera(d_camera_position, player_position, player_up);
	threed.update_v_matrix(v_matrix);
	
	render();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') {
		player_position += (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
	}
	if (key == 's') {
		player_position -= (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
	}	
	
	if (key == 'a') {
		player_direction -= 0.1;
	}
	if (key == 'd') {
		player_direction += 0.1;
	}

	player.transform = IDENTITY.translated(player_position).rotated_3d(player_up * -1, player_direction);
	
	camera_position = (Vector)player_position;
	camera_position += IDENTITY.rotated_3d(player_up, player_direction).get_vector() * -camera_follow;
	camera_position += (player_up * camera_height);

	//render();
}

void draw() {
	threed.clear_depth_buffer();
	
	threed.draw_model_3d(player, get_byte_color(255, 0, 0));
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
	//load_model("sphere.obj", &player);
	//load_model("cube.obj", &test_cube);
	
	//test_model.transform = test_model.transform.rotated_3d_z(1.5);
	//test_model.transform = test_model.transform.translated(10, 0, 0);
	
	//test_cube = create_face((Vector){0,1,0},(Vector){1,1,0},(Vector){1,0,0},(Vector){0,0,0});
	//test_model = create_face((Vector){0,1,1},(Vector){1,1,1},(Vector){1,1,0},(Vector){0,1,0});
	
	test_cube = create_box((Vector){0,0,0}, 1, 2, 3);
	player = create_box((Vector){0,0,0}, 1, 1, 1);
	//test_cube.print();
	
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

