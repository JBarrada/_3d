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

double camera_follow = 10.0;
double camera_height = 5.0;
double smooth_speed = 10.0;

Vector player_position(0, 0, 0);
Vector player_up(0, 0, 1);
double player_direction = 0;
Vector camera_position(15,15,3);

Vector d_camera_position(15,15,3);
Vector d_player_up(0, 0, 1);
Vector d_player_position(0, 0, 0);


Model player;

Model x_cube;
Model y_cube;
Model z_cube;

Model monkey;

Level test_level;


void idle() {
	d_camera_position.x += (camera_position.x - d_camera_position.x) / smooth_speed;
	d_camera_position.y += (camera_position.y - d_camera_position.y) / smooth_speed;
	d_camera_position.z += (camera_position.z - d_camera_position.z) / smooth_speed;

	d_player_up.x += (player_up.x - d_player_up.x) / smooth_speed;
	d_player_up.y += (player_up.y - d_player_up.y) / smooth_speed;
	d_player_up.z += (player_up.z - d_player_up.z) / smooth_speed;
	
	d_player_up.norm();
	
	d_player_position.x += (player_position.x - d_player_position.x) / smooth_speed * 2.0;
	d_player_position.y += (player_position.y - d_player_position.y) / smooth_speed * 2.0;
	d_player_position.z += (player_position.z - d_player_position.z) / smooth_speed * 2.0;
	
	
	player.transform = IDENTITY.translated(d_player_position).rotated_3d(player_up, player_direction);
	
	
	//camera_position = (Vector)d_player_position;
	//camera_position += IDENTITY.rotated_3d(player_up * -1, player_direction).get_vector() * -camera_follow;
	//camera_position += (player_up * camera_height);
	Matrix camera_matrix((Vector){cos(player_direction)*-camera_follow, sin(player_direction)*-camera_follow, camera_height});
	if (!(player_up.x == 0 && player_up.y == 0 && player_up.z == 1)) {
		Vector axis = player_up.cross((Vector){0,0,1});
		double cos_angle = player_up.dot((Vector){0,0,1});
		camera_matrix = camera_matrix.rotated_3d(axis, cos_angle+1, cos_angle);
	}
	
	camera_position = camera_matrix.get_vector() + (Vector)d_player_position;
	
	
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
		player_position = test_level.world_pos;
		player_up = test_level.world_up;
		/*
		test_level.move(step);
		player_position = test_level.world_position;
		player_up = test_level.surfaces[test_level.current_surface].up;
		player_position += ((Vector)player_up * 0.5);
		*/
	}
	if (key == 's') {
		//player_position -= (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
		Vector step(cos(player_direction)*-0.2, sin(player_direction)*-0.2, 0);
		
		test_level.move(step);
		player_position = test_level.world_pos;
		player_up = test_level.world_up;
		/*
		test_level.move(step * -1);
		player_position = test_level.world_position;
		player_up = test_level.surfaces[test_level.current_surface].up;
		player_position += ((Vector)player_up * 0.5);
		*/
	}	
	
	if (key == 'a') {
		player_direction += 0.1;
	}
	if (key == 'd') {
		player_direction -= 0.1;
	}
}

void draw() {
	threed.clear_depth_buffer();
	
	threed.draw_model_3d(player);
	
	threed.draw_model_3d(test_level.m);
	
	//threed.draw_model_3d(monkey);
	
	threed.draw_model_3d(x_cube);
	threed.draw_model_3d(y_cube);
	threed.draw_model_3d(z_cube);
	
	//shade(threed.depth_buffer);
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
	//load_model("tree2.obj", &monkey);
	//monkey.materials[0].color = 0x0b156b; // bark
	//monkey.materials[1].color = 0x199916; // leaves
	
	//monkey.transform = IDENTITY.translated(2,2,0.4);
	//monkey.transform = monkey.transform.rotated_3d_x(M_PI/2);
	//monkey.transform = monkey.transform.scaled(0.5, 0.5, 0.5);

	
	player = create_box((Vector){0,0,0}, 0.5, 0.5, 0.5, 0xff0000);
	//load_model("sphere.obj", &player);
	player.materials[0].color = 0xff2222;
	
	x_cube = create_box((Vector){1,0,0}, 0.2, 0.2, 0.2, 0x0000ff);
	y_cube = create_box((Vector){0,1,0}, 0.2, 0.2, 0.2, 0x00ff00);
	z_cube = create_box((Vector){0,0,1}, 0.2, 0.2, 0.2, 0xff0000);
	
	/*
	Model plane = create_face((Vector){0,0,0}, 8, 8, (Vector){0,0,1}, 0xffffff);
	plane.materials_count = 2;
	plane.materials = new Material[2];
	char test_mtl[] = "TEST";
	plane.materials[0] = (Material) {test_mtl, 0xff00ff};
	plane.materials[1] = (Material) {test_mtl, 0xffff00};
	plane.triangles[0].mtl = 0;
	plane.triangles[1].mtl = 1;
	test_level = Level(plane);
	*/
	
	Model cube = create_box((Vector){-4,4,-4}, 8, 8, 8, 0xffffff);
	
	cube.materials_count = 4;
	cube.materials = new Material[4];
	char test_mtl[] = "TEST";
	cube.materials[0] = (Material) {test_mtl, 0xffffff};
	cube.materials[1] = (Material) {test_mtl, 0x0000ff};
	cube.materials[2] = (Material) {test_mtl, 0x00ff00};
	cube.materials[3] = (Material) {test_mtl, 0xff0000};
	cube.triangles[3].mtl = 1;
	
	test_level = Level(cube);
	test_level.current_surface = 3;
	//test_level.m.print();
	
	
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

