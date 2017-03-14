#include "matrix.h"
#include "vector.h"
#include "math.h"
#include "threed.h"
#include "gfx.h"

#include <stdio.h>

ThreeD threed;

Vector translate(10,10,-20);
Vector rotate(0,0,0);

double angle = 0.0;

void idle() {
	
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') {
		translate += (Vector){0, 0.1, 0};
	}
	if (key == 's') {
		translate += (Vector){0, -0.1, 0};
	}
	
	if (key == 'a') {
		translate += (Vector){-0.1, 0, 0};
		angle -= 0.1;
	}
	if (key == 'd') {
		translate += (Vector){0.1, 0, 0};
		angle += 0.1;
	}
	
	double mx = cos(angle) * 20.0;
	double my = sin(angle) * 20.0;
	
	//printf("%f, %f\n", mx, my);
	
	double v_x[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
	Matrix v_matrix(4, 4, v_x);
	
	v_matrix.translate(mx, my, 5);
	//Matrix v_matrix = look_at_camera((Vector){mx, 10.0, my}, (Vector){0,0,0}, (Vector){0.0,1.0,0.0});

	threed.pv_matrix = (threed.p_matrix * v_matrix);
	render();
}

void draw() {
	threed.clear_depth_buffer();
	
	//threed.draw_line((Vector){10,10,0}, (Vector){20,30,0}, 50);
	
	threed.draw_point_3d((Vector){0,0,0},20);
	threed.draw_point_3d((Vector){0,1,0},50);
	threed.draw_point_3d((Vector){1,1,0},50);
	threed.draw_point_3d((Vector){1,0,0},50);
	
	threed.draw_point_3d((Vector){0,0,1},50);
	threed.draw_point_3d((Vector){0,1,1},50);
	threed.draw_point_3d((Vector){1,1,1},50);
	threed.draw_point_3d((Vector){1,0,1},50);
	
	threed.draw_line_3d((Vector){0,0,0}, (Vector){0,1,0}, 20);
	threed.draw_line_3d((Vector){0,1,0}, (Vector){1,1,0}, 20);
	threed.draw_line_3d((Vector){1,1,0}, (Vector){1,0,0}, 20);
	threed.draw_line_3d((Vector){1,0,0}, (Vector){0,0,0}, 20);
	
}

int main() {
	//Matrix p_matrix = projection_ortho(3.2/10.0, 2.0/10.0, 100.0, 1.0);
	//Matrix p_matrix = projection_persp(10.0, 10.0, 100.0, 1.0);
	Matrix p_matrix = projection_persp_gl(90.0, 100.0, 0.1, 320.0/200.0);
	double v_x[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
	Matrix v_matrix(4, 4, v_x);
	
	//v_matrix.rotate_3d_x(-1.5708);
	//v_matrix.rotate_3d_y(-0.785398);
	//v_matrix.rotate_3d_z(0.785398);
	//v_matrix.translate(0, -5, -10);
	
	v_matrix.print();
	
	threed.init(p_matrix, v_matrix, 3.2, 2.0);
	
	gfx_init(idle, draw, keyboard);
	
	return 0;
}

