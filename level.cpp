#include "level.h"

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
		Vector axis = up.cross((Vector){0,0,1});
		double cos_angle = up.dot((Vector){0,0,1});
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



Level::Level() {
	current_surface = 0;
	surface_position = (Vector){1,1,0};
	world_position = (Vector){1,1,0};
}

bool Level::move(Vector step) {
	Vector next = surface_position + step;
	
	if (next.x < 0) {
		if (surfaces[current_surface].l != -1) {
			surface_position = next;
			current_surface = surfaces[current_surface].l;
			surface_position.x += surfaces[current_surface].width;
			world_position = surfaces[current_surface].world_pos(surface_position);
			return true;
		} else {
			return false;
		}
	}
	if (next.x >= surfaces[current_surface].width) {
		if (surfaces[current_surface].r != -1) {
			surface_position = next;
			surface_position.x -= surfaces[current_surface].width;
			current_surface = surfaces[current_surface].r;
			world_position = surfaces[current_surface].world_pos(surface_position);
			return true;
		} else {
			return false;
		}
	}
	if (next.y < 0) {
		if (surfaces[current_surface].d != -1) {
			surface_position = next;
			current_surface = surfaces[current_surface].d;
			surface_position.y += surfaces[current_surface].height;
			world_position = surfaces[current_surface].world_pos(surface_position);
			return true;
		} else {
			return false;
		}
	}
	if (next.y >= surfaces[current_surface].height) {
		if (surfaces[current_surface].u != -1) {
			surface_position = next;
			surface_position.y -= surfaces[current_surface].height;
			current_surface = surfaces[current_surface].u;
			world_position = surfaces[current_surface].world_pos(surface_position);
			return true;
		} else {
			return false;
		}
	}
	
	surface_position = next;
	world_position = surfaces[current_surface].world_pos(surface_position);
	
	return false;
}