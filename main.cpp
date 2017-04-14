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

bool keys[4] = {false, false, false, false};

double camera_follow = 15.0;
double camera_height = 5.0;
double smooth_speed = 10.0;
double camera_smooth_speed = 40.0;

Vector player_position(0, 0, 0);
Vector player_up(0, 0, 1);
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

// debug stuff
Model debug_player;
Model debug;
Model* world_points = new Model[3];
Model* surface_points = new Model[3];
Model* surface_world_points = new Model[3];
int debug_triangle = -1;
bool hide_level = false;


void idle() {
	bool update_debug = false;
	if (keys[0]) {
		update_debug = test_level.move(true);
		player_position = test_level.world_pos;
		player_up = test_level.world_up;
		player_position += ((Vector)player_up * 0.25);

	}
	if (keys[2]) {
		update_debug = test_level.move(false);
		player_position = test_level.world_pos;
		player_up = test_level.world_up;
		player_position += ((Vector)player_up * 0.25);
	}	
	if (keys[1]) {
		test_level.player_dir += 0.05;
	}
	if (keys[3]) {
		test_level.player_dir -= 0.05;
	}
	
	
	d_camera_position.x += (camera_position.x - d_camera_position.x) / camera_smooth_speed;
	d_camera_position.y += (camera_position.y - d_camera_position.y) / camera_smooth_speed;
	d_camera_position.z += (camera_position.z - d_camera_position.z) / camera_smooth_speed;

	d_player_up.x += (player_up.x - d_player_up.x) / camera_smooth_speed;
	d_player_up.y += (player_up.y - d_player_up.y) / camera_smooth_speed;
	d_player_up.z += (player_up.z - d_player_up.z) / camera_smooth_speed;
	
	d_player_up.norm();
	
	d_player_position.x += (player_position.x - d_player_position.x) / smooth_speed * 2.0;
	d_player_position.y += (player_position.y - d_player_position.y) / smooth_speed * 2.0;
	d_player_position.z += (player_position.z - d_player_position.z) / smooth_speed * 2.0;
	
	
	//player.transform = IDENTITY.translated(d_player_position).rotated_3d(player_up, test_level.player_dir);
	Vector default_up(0,0,1);
	player.transform = IDENTITY.translated(d_player_position) * align_vectors(default_up, player_up) * IDENTITY.rotated_3d_z(test_level.player_dir);
	
	// debug
	debug_player.transform = IDENTITY.translated(test_level.surface_pos);
	
	//camera_position = (Vector)d_player_position;
	//camera_position += IDENTITY.rotated_3d(player_up * -1, player_direction).get_vector() * -camera_follow;
	//camera_position += (player_up * camera_height);
	Matrix camera_matrix((Vector){cos(test_level.player_dir)*-camera_follow, sin(test_level.player_dir)*-camera_follow, camera_height});
	/*
	if (!(player_up.x == 0 && player_up.y == 0 && player_up.z == 1)) {
		Vector axis = player_up.cross((Vector){0,0,1});
		double cos_angle = player_up.dot((Vector){0,0,1});
		double angle = acos(cos_angle);
		camera_matrix = camera_matrix.rotated_3d(axis, angle);
	}
	*/
	
	if (player_up == default_up) {
		
	} else if (player_up == (default_up * -1)) {
		Vector axis = player_up.cross((Vector){0,1,0});
		double angle = M_PI;
		camera_matrix = camera_matrix.rotated_3d(axis, angle);
	} else {
		Vector axis = player_up.cross((Vector){0,0,1});
		double cos_angle = player_up.dot((Vector){0,0,1});
		double angle = acos(cos_angle);
		camera_matrix = camera_matrix.rotated_3d(axis, angle);
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

void keyboard_down(unsigned char key, int x, int y) {
	switch(key) {
		case 'w': keys[0] = true; break;
		case 'a': keys[1] = true; break;
		case 's': keys[2] = true; break;
		case 'd': keys[3] = true; break;
	}
}
void keyboard_up(unsigned char key, int x, int y) {
	switch(key) {
		case 'w': keys[0] = false; break;
		case 'a': keys[1] = false; break;
		case 's': keys[2] = false; break;
		case 'd': keys[3] = false; break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	
	bool update_debug = false;
	
	if (key == 'w') {
		//player_position += (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
		//Vector step(cos(player_direction)*0.2, sin(player_direction)*0.2, 0);
		
		update_debug = test_level.move(true);
		player_position = test_level.world_pos;
		player_up = test_level.world_up;
		player_position += ((Vector)player_up * 0.25);
		//player_direction = test_level.player_dir;
		/*
		test_level.move(step);
		player_position = test_level.world_position;
		player_up = test_level.surfaces[test_level.current_surface].up;
		player_position += ((Vector)player_up * 0.5);
		*/
	}
	if (key == 's') {
		//player_position -= (IDENTITY.rotated_3d(player_up, player_direction).get_vector() * 0.2);
		//Vector step(cos(player_direction)*-0.2, sin(player_direction)*-0.2, 0);
		
		update_debug = test_level.move(false);
		player_position = test_level.world_pos;
		player_up = test_level.world_up;
		player_position += ((Vector)player_up * 0.25);
		//player_direction = test_level.player_dir;
		/*
		test_level.move(step * -1);
		player_position = test_level.world_position;
		player_up = test_level.surfaces[test_level.current_surface].up;
		player_position += ((Vector)player_up * 0.5);
		*/
	}	
	
	if (key == 'a') {
		test_level.player_dir += 0.1;
	}
	if (key == 'd') {
		test_level.player_dir -= 0.1;
	}
	
	if (key == 'q') {
		debug_triangle--;
		printf("DEBUG: %d\n", debug_triangle);
	}
	if (key == 'e') {
		debug_triangle++;
		printf("DEBUG: %d\n", debug_triangle);
	}
	if (key == 'h') {
		hide_level = !hide_level;
	}
	
	if (update_debug) {
		debug_triangle = test_level.current_surface;
		
	//if (debug_triangle >= 0 && debug_triangle < debug.triangles_count) {
		//test_level.m.normals[test_level.m.triangles[debug_triangle].normal].print();
		Vector a = test_level.transform_n_to_s(debug.points[debug.triangles[debug_triangle].a], debug_triangle);
		Vector b = test_level.transform_n_to_s(debug.points[debug.triangles[debug_triangle].b], debug_triangle);
		Vector c = test_level.transform_n_to_s(debug.points[debug.triangles[debug_triangle].c], debug_triangle);
		
		Vector a1 = test_level.transform_s_to_n(a, debug_triangle);
		Vector b1 = test_level.transform_s_to_n(b, debug_triangle);
		Vector c1 = test_level.transform_s_to_n(c, debug_triangle);
		//a.print();
		//b.print();
		//c.print();
		
		surface_points[0] = create_box(a, 0.2, 0.2, 1, 0x0000ff);
		surface_points[1] = create_box(b, 0.2, 0.2, 1, 0x00ff00);
		surface_points[2] = create_box(c, 0.2, 0.2, 1, 0xff0000);
		
		surface_world_points[0] = create_box(a1, 0.2, 0.2, 0.2, 0x000088);
		surface_world_points[1] = create_box(b1, 0.2, 0.2, 0.2, 0x008800);
		surface_world_points[2] = create_box(c1, 0.2, 0.2, 0.2, 0x880000);
		
		world_points[0] = create_box(debug.points[debug.triangles[debug_triangle].a], 0.5, 0.5, 0.5, 0x5555ff);
		world_points[1] = create_box(debug.points[debug.triangles[debug_triangle].b], 0.5, 0.5, 0.5, 0x55ff55);
		world_points[2] = create_box(debug.points[debug.triangles[debug_triangle].c], 0.5, 0.5, 0.5, 0xff5555);
	}
}

void debug_init() {
	int world_points_pos = 0;
	
	for (int t=0; t<debug.triangles_count; t++) {
		
		
		
		world_points[world_points_pos++] = create_box(debug.points[debug.triangles[t].a] + (debug.normals[debug.triangles[t].normal] * 0.1), 0.2, 0.2, 0.2, 0x0000ff);
		world_points[world_points_pos++] = create_box(debug.points[debug.triangles[t].b] + (debug.normals[debug.triangles[t].normal] * 0.1), 0.2, 0.2, 0.2, 0x00ff00);
		world_points[world_points_pos++] = create_box(debug.points[debug.triangles[t].c] + (debug.normals[debug.triangles[t].normal] * 0.1), 0.2, 0.2, 0.2, 0xff0000);
	}
	
	int surface_points_pos = 0;
	for (int t=0; t<debug.triangles_count; t++) {
		Vector a = (Vector)debug.points[debug.triangles[t].a];
		Vector b = (Vector)debug.points[debug.triangles[t].b];
		Vector c = (Vector)debug.points[debug.triangles[t].c];

		/*
		a.print();
		b.print();
		c.print();
		*/

		// normal transform to surface space
		a = (test_level.n_to_s[debug.triangles[t].normal] * Matrix(a)).get_vector();
		b = (test_level.n_to_s[debug.triangles[t].normal] * Matrix(b)).get_vector();
		c = (test_level.n_to_s[debug.triangles[t].normal] * Matrix(c)).get_vector();

		a.z = t*0.1;
		b.z = t*0.1;
		c.z = t*0.1;
		
		a.print();
		b.print();
		c.print();
		
		surface_points[surface_points_pos++] = create_box(a, 0.2, 0.2, 0.2, 0x0000ff);
		surface_points[surface_points_pos++] = create_box(b, 0.2, 0.2, 0.2, 0x00ff00);
		surface_points[surface_points_pos++] = create_box(c, 0.2, 0.2, 0.2, 0xff0000);
	}
}

void draw() {
	threed.clear_depth_buffer();
	
	threed.draw_model_3d(player);
	
	if (!hide_level)
		threed.draw_model_3d(test_level.m);
	
	for (int i=0; i < test_level.deco_count; i++) {
		threed.draw_model_3d(test_level.deco[i]);
	}
	
	//threed.draw_model_3d(monkey);
	
	threed.draw_model_3d(x_cube);
	threed.draw_model_3d(y_cube);
	threed.draw_model_3d(z_cube);
	
	
	if (debug_triangle >= 0 && debug_triangle < debug.triangles_count) {
		for (int p=0; p<3; p++) {
			threed.draw_model_3d(world_points[p]);
			threed.draw_model_3d(surface_points[p]);
			threed.draw_model_3d(surface_world_points[p]);
		}
	}
	
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

	
	//debug_player = create_box((Vector){0,0,0}, 0.5, 0.5, 0.5, 0xff0000);
	

	//debug = create_box((Vector){-4,4,-4}, 8, 8, 8, 0xffffff);
	
	load_model("test_level.obj", &debug);
	test_level = Level(debug);
	//test_level.current_surface = 3;
	
	test_level.deco_count = 1;
	test_level.deco = new Model[1];
	load_model("test_level_grass.obj", &test_level.deco[0]);
	//test_level.deco[0].backface_cull = true;
	
	//debug_init();
	
	
	Matrix p_matrix = projection_persp_gl(120.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	threed.backface_cull = true;
	
	gfx_init(idle, draw, keyboard_down, keyboard_up);
	
	return 0;
}

