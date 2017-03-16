#ifndef GFX_H
#define GFX_H

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <string.h>
#include "math.h"
#include <stdint.h>
#include <stdio.h>

#define SCREEN_W 320
#define SCREEN_H 200

struct point {
	double x, y;
};
	
void gfx_init(void (*idle)(), void (*draw)(), void (*keyboard)(unsigned char key, int x, int y));

uint8_t get_byte_color(uint8_t r, uint8_t g, uint8_t b);
uint8_t get_byte_color(uint32_t color);
uint8_t interpolate_color(uint8_t c, double value);

void set_pixel(int, int, uint8_t);

void shade(double*); 
void toon(double*);

void line(int, int, int, int, uint8_t);
void circle(int, int, int, uint8_t);
void circle_filled(int, int, int, uint8_t);
void polygon_filled(point[], int, uint8_t);
void rectangle_filled(int, int, int, int, uint8_t);

void render();

#endif
