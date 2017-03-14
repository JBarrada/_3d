#include "gfx.h"

uint32_t palette[256];
uint32_t screen_buffer[SCREEN_W*SCREEN_H];

void (*draw_function)();


void gfx_init(void (*idle)(), void (*draw)(), void (*keyboard)(unsigned char key, int x, int y)) {
	draw_function = draw;
	
	for (int i=0; i<32; i++) {palette[i] = ((int)(i*2.09375)<<16) | ((int)(i*6.46875)<<8) | (int)(i*7.90625);}
	for (int i=0; i<32; i++) {palette[i+32] = ((int)(i*0.4375)<<16) | ((int)(i*1.875)<<8) | (int)(i*7.3125);}
	for (int i=0; i<32; i++) {palette[i+64] = ((int)(i*7.21875)<<16) | ((int)(i*7.3125)<<8) | (int)(i*3.15625);}
	for (int i=0; i<32; i++) {palette[i+96] = ((int)(i*7.9375)<<16) | ((int)(i*4.28125)<<8) | (int)(i*1.6875);}
	for (int i=0; i<32; i++) {palette[i+128] = ((int)(i*4.125)<<16) | ((int)(i*2.21875)<<8) | (int)(i*0.375);}
	
	char fakeParam[] = "";
	char *fakeargv[] = { fakeParam, NULL };
	int fakeargc = 1;
	
	glutInit(&fakeargc, fakeargv);
	glutInitWindowSize(SCREEN_W, SCREEN_H);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("!");
	glutIdleFunc(idle);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();
}

void render() {
	memset(screen_buffer, 0, sizeof(screen_buffer));
	
	draw_function();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawPixels(SCREEN_W, SCREEN_H, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, screen_buffer);
	glutSwapBuffers();
}

void set_pixel(int x, int y, uint8_t c) {
	if (x<SCREEN_W & y<SCREEN_H & x>=0 & y>=0) {
		screen_buffer[(y*SCREEN_W+x)] = palette[c];
	}	
}

void line(int x1, int y1, int x2, int y2, uint8_t c) {
	int dx, dy, dxabs, dyabs, x, y, px, py;
	dx = (x2-x1);
	dy = (y2-y1);
	dxabs = abs(dx);
	dyabs = abs(dy);
	
	if (dyabs < 1000 & dxabs < 1000) {
		x=dyabs>>1;
		y=dxabs>>1;
		px=x1;
		py=y1;
		
		int i;
		if (dxabs>=dyabs) {
			for(i=0;i<dxabs;i++) {
				y+=dyabs;
				if (y>=dxabs) {
					y-=dxabs;
					py+=(dy>=0)?1:-1;
				}
				px+=(dx>=0)?1:-1;
				set_pixel(px,py,c);
			}
		}
		else {
			for(i=0;i<dyabs;i++) {
				x+=dxabs;
				if (x>=dyabs) {
					x-=dyabs;
					px+=(dx>=0)?1:-1;
				}
				py+=(dy>=0)?1:-1;
				set_pixel(px,py,c);
			}
		}
	}
}

void circle(int cx, int cy, int r, uint8_t c) {
	int i, res=10;
	double x, y, x_, y_;
	for (i=0; i<=res; i++) {
		x = r * cos(((M_PI/2)/res)*i);
		y = r * sin(((M_PI/2)/res)*i);
		
		if (i != 0) {
			line(cx+x_, cy+y_, cx+x, cy+y, c);
			line(cx-x_, cy+y_, cx-x, cy+y, c);
			line(cx+x_, cy-y_, cx+x, cy-y, c);
			line(cx-x_, cy-y_, cx-x, cy-y, c);
		}
		
		x_ = x;
		y_ = y;
	}
}

void circle_filled(int cx, int cy, int r, uint8_t c) {
	int i, tx, ty;
	int r2 = r * r;
	int area = r2 << 2;
	int rr = r << 1;

	for (i=0; i<area; i++)
	{
		tx = (i % rr) - r;
		ty = (i / rr) - r;

		if (tx * tx + ty * ty <= r2)
			set_pixel(cx+tx, cy+ty, c);
	}
}

void polygon_filled(point points[], int num_points, uint8_t c) {
	int poly_top, poly_bot, poly_left, poly_right, swap, p_x, p_y, node_x[num_points], i, j, nodes;
	for (i=0; i<num_points; i++) {
		if (!i) {
			poly_top = poly_bot = points[i].y;
			poly_left = poly_right = points[i].x;
		}
		else {
			if (points[i].y > poly_top)
				poly_top = points[i].y;
			if (points[i].y < poly_bot)
				poly_bot = points[i].y;
			if (points[i].x > poly_right)
				poly_right = points[i].x;
			if (points[i].x < poly_left)
				poly_left = points[i].x;
		}
	}
	
	// loop through rows of the image
	for (p_y=poly_bot; p_y<poly_top; p_y++) {
		
		// build a list of nodes
		nodes = 0;
		j = num_points - 1;
		for (i=0; i<num_points; i++) {
			if (points[i].y<(double) p_y && points[j].y>=(double) p_y ||  points[j].y<(double) p_y && points[i].y>=(double) p_y) {
				node_x[nodes++] = (int) (points[i].x + (p_y-points[i].y) / (points[j].y-points[i].y) * (points[j].x-points[i].x)); 
			}
			j=i; 
		}
		
		// sort the nodes, via a simple �bubble� sort
		i=0;
		while (i<nodes-1) {
			if (node_x[i] > node_x[i+1]) {
				swap = node_x[i]; 
				node_x[i] = node_x[i+1]; 
				node_x[i+1] = swap; 
				if (i) 
					i--; 
			}
			else {
				i++; 
			}
		}
		
		//  Fill the pixels between node pairs.
		for (i=0; i<nodes; i+=2) {
			
			if (node_x[i] >= poly_right) 
				break;
			
			if (node_x[i+1] > poly_left) {
				if (node_x[i] < poly_left) 
					node_x[i] = poly_left;
				
				if (node_x[i+1] > poly_right) 
					node_x[i+1] = poly_right;
				
				for (p_x=node_x[i]; p_x<node_x[i+1]; p_x++) 
					set_pixel(p_x, p_y, c);
			}
		}
	}
}

void rectangle_filled(int x, int y, int width, int height, uint8_t c) {
	point rect[4];
	rect[0] = (point) {x, y};
	rect[1] = (point) {x+width, y};
	rect[2] = (point) {x+width, y+height};
	rect[3] = (point) {x, y+height};
	polygon_filled(rect, 4, c);
}