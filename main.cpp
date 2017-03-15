#include "matrix.h"
#include "vector.h"
#include "math.h"
#include "threed.h"
#include "gfx.h"

#include <stdio.h>

ThreeD threed;

Vector camera_position(10,10,1);
double angle = 0;

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
	
	camera_position.x = cos(angle) * 10.0;
	camera_position.y = sin(angle) * 10.0;
	
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	threed.update_v_matrix(v_matrix);

	render();
}

void draw() {
	threed.clear_depth_buffer();
	
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
	
	
}

int main() {
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	Matrix v_matrix = look_at_camera(camera_position, (Vector){0,0,0}, (Vector){0,0,1});
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

