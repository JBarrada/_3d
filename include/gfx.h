#ifndef GFX_H
#define GFX_H

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <string.h>
#include "math.h"
#include <stdint.h>
#include <stdio.h>

#define SCREEN_W 640
#define SCREEN_H 400

struct point {
	double x, y;
};
	
void gfx_init(void (*idle)(), void (*draw)(), void (*keyboard)(unsigned char key, int x, int y));

uint8_t get_byte_color(uint8_t r, uint8_t g, uint8_t b);
uint32_t get_32bit_color(uint8_t r, uint8_t g, uint8_t b);
uint8_t get_byte_color(uint32_t color);
uint32_t interpolate_color_32bit(uint32_t c, double value);
uint8_t interpolate_color(uint8_t c, double value);

void set_pixel(int, int, uint8_t);
void set_pixel_32bit(int x, int y, uint32_t c);

void shade(double*); 
void toon(double*);
void dither();

void toon_set_pixel(int x, int y, uint8_t c);
void toon_mask_line(int x0, int y0, int x1, int y1, float wd);

void line(int, int, int, int, uint8_t);
void circle(int, int, int, uint8_t);
void circle_filled(int, int, int, uint8_t);
void polygon_filled(point[], int, uint8_t);
void rectangle_filled(int, int, int, int, uint8_t);

void render();

#endif
