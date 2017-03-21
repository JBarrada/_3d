#include "matrix.h"
#include "vector.h"
#include "math.h"
#include "threed.h"
#include "gfx.h"
#include "model.h"
#include "level.h"

#include <stdio.h>

#include <iostream>
#include <fstream>

ThreeD threed;

Vector player_position(0, 0, 0);
Vector player_up(0, 0, 1);
double player_direction = 0;
Vector camera_position(15,15,1);
double camera_follow = 8.0;
double camera_height = 2.0;

Vector d_camera_position(15,15,1);
Vector d_player_up(0, 0, 1);
Vector d_player_position(0, 0, 0);

Vector surface_position(1,1,0);

double smooth_speed = 10.0;

Model player;

Model x_cube;
Model y_cube;
Model z_cube;

Surface test_surface1((Vector){0,0,0},(Vector){0,0,1}, 8, 8, -1, 1, -1, -1);
Surface test_surface2((Vector){0,0,-8},(Vector){0,-1,0}, 8, 8, 0, -1, -1, -1);

Level test_level;



void idle() {
	d_camera_position.x += (camera_position.x - d_camera_position.x) / smooth_speed;
	d_camera_position.y += (camera_position.y - d_camera_position.y) / smooth_speed;
	d_camera_position.z += (camera_position.z - d_camera_position.z) / smooth_speed;
	

	d_player_up.x += (player_up.x - d_player_up.x) / smooth_speed;
	d_player_up.y += (player_up.y - d_player_up.y) / smooth_speed;
	d_player_up.z += (player_up.z - d_player_up.z) / smooth_speed;
	
	d_player_position.x += (player_position.x - d_player_position.x) / smooth_speed;
	d_player_position.y += (player_position.y - d_player_position.y) / smooth_speed;
	d_player_position.z += (player_position.z - d_player_position.z) / smooth_speed;
	
	
	Matrix v_matrix = look_at_camera(d_camera_position, d_player_position, d_player_up);
	threed.update_v_matrix(v_matrix);
	
	// update near plane
	threed.np_n = (d_player_position - d_camera_position).norm();
	threed.np_d = threed.np_n.dot(d_camera_position + threed.np_n);
	
	threed.camera_pos = (Vector)(d_camera_position + threed.np_n);
	
	//threed.np_n.print();
	//printf("%f\n", threed.np_d);
	
	render();
}

void keyboard(unsigned char key, int x, int y) {
	
	if (key == 'w') {
		//player_position += (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
		Vector step(cos(player_direction)*0.2, sin(player_direction)*0.2, 0);
		
		test_level.move(step);
		player_position = test_level.world_position;
		player_up = test_level.surfaces[test_level.current_surface].up;
	}
	if (key == 's') {
		//player_position -= (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
		Vector step(cos(player_direction)*0.2, sin(player_direction)*0.2, 0);
		
		test_level.move(step * -1);
		player_position = test_level.world_position;
		player_up = test_level.surfaces[test_level.current_surface].up;
	}	
	
	if (key == 'a') {
		player_direction += 0.1;
	}
	if (key == 'd') {
		player_direction -= 0.1;
	}
	
	if (key == 'q') {
		//player_up = (Vector){0,0,1};
	}
	if (key == 'e') {
		//player_up = (Vector){0,1,0};
	}
	

	player.transform = IDENTITY.translated(player_position).rotated_3d(player_up, player_direction);
	
	camera_position = (Vector)player_position;
	camera_position += IDENTITY.rotated_3d(player_up * -1, player_direction).get_vector() * -camera_follow;
	camera_position += (player_up * camera_height);

	//render();
}

void draw() {
	threed.clear_depth_buffer();
	
	threed.draw_model_3d(player, get_32bit_color(255, 0, 0));
	
	for (int i=0; i < test_level.surfaces_count; i++) {
		threed.draw_model_3d(test_level.surfaces[i].m, get_32bit_color(255, 255, 255));
	}
	
	/*
	printf("START-SURFACE\n");
	threed.draw_model_3d(test_level.surfaces[0].m, get_byte_color(255, 255, 255));
	printf("END-SURFACE\n");
	*/
	
	threed.draw_model_3d(x_cube, get_32bit_color(0, 0, 255));
	threed.draw_model_3d(y_cube, get_32bit_color(0, 255, 0));
	threed.draw_model_3d(z_cube, get_32bit_color(255, 0, 0));
	
	shade(threed.depth_buffer);
	toon(threed.depth_buffer);
	dither();
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
	//load_model("test.obj", &player);
	//load_model("cube.obj", &test_cube);

	//test_cube = create_face((Vector){0,0,0}, 4, 5, (Vector){0,0,1});
	player = create_box((Vector){0,0,0}, 0.5, 0.5, 0.5);
	//test_cube.print();
	
	x_cube = create_box((Vector){1,0,0}, 0.2, 0.2, 0.2);
	y_cube = create_box((Vector){0,1,0}, 0.2, 0.2, 0.2);
	z_cube = create_box((Vector){0,0,1}, 0.2, 0.2, 0.2);
	
	test_level.surfaces_count = 2;
	test_level.surfaces = new Surface[2];
	test_level.surfaces[0] = test_surface1;
	test_level.surfaces[1] = test_surface2;
	
	
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

