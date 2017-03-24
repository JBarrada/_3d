#include "level.h"
/*
Surface::Surface() {
	pos = (Vector){0,0,0};
	up = (Vector){0,0,1};
	
	width = 0;
	height = 0;
	
	u = -1;
	d = -1;
	l = -1;
	r = -1;
}

Surface::Surface(Vector pos, Vector up, double width, double height, uint32_t color) {
	this->pos = pos;
	this->up = up;
	
	this->width = width;
	this->height = height;
	
	this->color = color;
	
	this->u = -1;
	this->d = -1;
	this->l = -1;
	this->r = -1;
	
	surface_to_world = (Matrix)IDENTITY;
	if (!(up.x == 0 && up.y == 0 && up.z == 1)) {
		Vector axis = up.cross((Vector){0,0,-1});
		double cos_angle = up.dot((Vector){0,0,-1});
		surface_to_world = surface_to_world.rotated_3d(axis, cos_angle+1, cos_angle);
	}
	
	generate_face();
}

Surface::Surface(Vector pos, Vector up, double width, double height, int u, int d, int l, int r, uint32_t color) {
	this->pos = pos;
	this->up = up;
	
	this->width = width;
	this->height = height;
	
	this->color = color;
	
	this->u = u;
	this->d = d;
	this->l = l;
	this->r = r;
	
	surface_to_world = (Matrix)IDENTITY;
	if (!(up.x == 0 && up.y == 0 && up.z == 1)) {
		Vector axis = up.cross((Vector){0,0,-1});
		double cos_angle = up.dot((Vector){0,0,-1});
		surface_to_world = surface_to_world.rotated_3d(axis, cos_angle+1, cos_angle);
	}
	
	generate_face();
}

void Surface::generate_face() {
	Vector a = (Vector){0,0,0};
	Vector b = (Vector){0, height, 0};
	Vector c = (Vector){width, height, 0};
	Vector d = (Vector){width, 0, 0};
	
	a = (surface_to_world * Matrix(a)).get_vector() + pos;
	b = (surface_to_world * Matrix(b)).get_vector() + pos;
	c = (surface_to_world * Matrix(c)).get_vector() + pos;
	d = (surface_to_world * Matrix(d)).get_vector() + pos;
	
	m = create_face(a, b, c, d, color);
}

int Surface::can_move(const Vector& v) {
	if (v.x < 0) {
		return l;
	}
	if (v.x >= width) {
		return r;
	}
	if (v.y < 0) {
		return d;
	}
	if (v.y >= height) {
		return u;
	}
	return -2;
}

Vector Surface::world_pos(const Vector& v) {
	return (surface_to_world * Matrix(v)).get_vector() + pos;
}
*/


Level::Level() {

}

Level::Level(Model m) {
	this->m = m;
	
	current_surface = 0;
	surface_pos = (Vector){0,0,0};
	world_pos = (Vector){0,0,0};
	
	generate_level();
}

int Level::find_neighbor(int p1, int p2, int avoid) {
	for (int t=0; t<m.triangles_count; t++) {
		if (t != avoid) {
			int match_count = 0;
			if (p1 == m.triangles[t].a || p2 == m.triangles[t].a) match_count++;
			if (p1 == m.triangles[t].b || p2 == m.triangles[t].b) match_count++;
			if (p1 == m.triangles[t].c || p2 == m.triangles[t].c) match_count++;
			if (match_count >= 2) {
				return t;
			}
		}
	}
	return -1;
}

void Level::generate_level() {
	this->surfaces = new Surface[m.triangles_count];
	
	for (int t=0; t<m.triangles_count; t++) {
		this->surfaces[t].ab = find_neighbor(m.triangles[t].a, m.triangles[t].b, t);
		this->surfaces[t].bc = find_neighbor(m.triangles[t].b, m.triangles[t].c, t);
		this->surfaces[t].ca = find_neighbor(m.triangles[t].c, m.triangles[t].a, t);
	}
}

bool move(Vector& step) {
	Vector test_pos = (Vector)surface_pos + step;
	
	// make point a origin
	Vector a = (Vector)m.points[m.triangles[current_surface].a];
	Vector b = (Vector)m.points[m.triangles[current_surface].b];
	Vector c = (Vector)m.points[m.triangles[current_surface].c];
	// todo normal transform to surface space
	c -= a;
	b -= a;
	a -= a;
	
	
	
}
