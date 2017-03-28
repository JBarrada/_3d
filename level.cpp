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
	world_up = (Vector){0,0,1};
	
	generate_level();
	
	this->world_pos = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(this->surface_pos)).get_vector();
	this->world_up = (Vector)m.normals[m.triangles[current_surface].normal];
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
	
	this->s_to_n = new Matrix[m.normals_count];
	
	for (int n=0; n<m.normals_count; n++) {
		this->s_to_n[n] = (Matrix)IDENTITY;
		if (!(m.normals[n].x == 0 && m.normals[n].y == 0 && m.normals[n].z == 1)) {
			Vector axis = m.normals[n].cross((Vector){0,0,1});
			double cos_angle = m.normals[n].dot((Vector){0,0,1});
			this->s_to_n[n] = this->s_to_n[n].rotated_3d(axis, cos_angle+1, cos_angle);
		}
	}
	
	this->n_to_s = new Matrix[m.normals_count];
	
	Vector up(0,0,1);
	for (int n=0; n<m.normals_count; n++) {
		this->n_to_s[n] = (Matrix)IDENTITY;
		if (!(m.normals[n].x == 0 && m.normals[n].y == 0 && m.normals[n].z == 1)) {
			Vector axis = up.cross(m.normals[n]);
			double cos_angle = up.dot(m.normals[n]);
			this->n_to_s[n] = this->n_to_s[n].rotated_3d(axis, cos_angle+1, cos_angle);
		}
	}
	
}


float tri_sign(Vector& p1, Vector& p2, Vector& p3) {
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool point_in_triangle(Vector& pt, Vector& v1, Vector& v2, Vector& v3) {
  bool b1, b2, b3;

  float v1_v2_sign = tri_sign(pt, v1, v2);
  float v2_v3_sign = tri_sign(pt, v2, v3);
  float v3_v1_sign = tri_sign(pt, v3, v1);
  
  b1 = tri_sign(pt, v1, v2) < 0.0f;
  b2 = tri_sign(pt, v2, v3) < 0.0f;
  b3 = tri_sign(pt, v3, v1) < 0.0f;
  
  printf("v1_v2: %f v2_v3: %f v3_v1: %f ", v1_v2_sign, v2_v3_sign, v3_v1_sign);
  
  return ((b1 == b2) && (b2 == b3));
}

bool Level::move(Vector& step) {
	Vector test_pos = (Vector)surface_pos + step;
	
	Vector a = (Vector)m.points[m.triangles[current_surface].a];
	Vector b = (Vector)m.points[m.triangles[current_surface].b];
	Vector c = (Vector)m.points[m.triangles[current_surface].c];
	
	/*
	a.print();
	b.print();
	c.print();
	*/
	
	// normal transform to surface space
	a = (this->n_to_s[m.triangles[current_surface].normal] * Matrix(a)).get_vector();
	b = (this->n_to_s[m.triangles[current_surface].normal] * Matrix(b)).get_vector();
	c = (this->n_to_s[m.triangles[current_surface].normal] * Matrix(c)).get_vector();
	
	/*
	a.print();
	b.print();
	c.print();
	*/
	
	bool ab_sign = tri_sign(test_pos, a, b) < 0.0;
	bool bc_sign = tri_sign(test_pos, b, c) < 0.0;
	bool ca_sign = tri_sign(test_pos, c, a) < 0.0;
	
	bool majority_sign = (ab_sign + bc_sign + ca_sign) > 1;
	
	if ((ab_sign == bc_sign) && (bc_sign == ca_sign)) {
		this->surface_pos.x = test_pos.x;
		this->surface_pos.y = test_pos.y;
		
		this->world_pos = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(this->surface_pos)).get_vector();
		this->world_up = (Vector)m.normals[m.triangles[current_surface].normal];
		return false;
	} else {
		if (ab_sign != majority_sign) {
			if (surfaces[current_surface].ab == -1) {
				return false;
			} else {
				current_surface = surfaces[current_surface].ab;
			}
		}else if (bc_sign != majority_sign) {
			if (surfaces[current_surface].bc == -1) {
				return false;
			} else {
				current_surface = surfaces[current_surface].bc;
			}
		}else if (ca_sign != majority_sign) {
			if (surfaces[current_surface].ca == -1) {
				return false;
			} else {
				current_surface = surfaces[current_surface].ca;
			}
		}
	}
	
	for (int t=0; t<m.triangles_count; t++) {
		if (t == current_surface) {
			m.triangles[t].mtl = 0;
		} else if (t == surfaces[current_surface].ab) {
			m.triangles[t].mtl = 1;
		} else if (t == surfaces[current_surface].bc) {
			m.triangles[t].mtl = 2;
		} else if (t == surfaces[current_surface].ca) {
			m.triangles[t].mtl = 3;
		} else {
			m.triangles[t].mtl = 0;
		}
	}
	
	printf("ab: %d bc: %d bc: %d \n", ab_sign, bc_sign, ca_sign);
	
	this->surface_pos.x = test_pos.x;
	this->surface_pos.y = test_pos.y;
	
	this->world_pos = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(this->surface_pos)).get_vector();
	this->world_up = (Vector)m.normals[m.triangles[current_surface].normal];
	
	surface_pos.print();
	world_pos.print();
	printf("CURRENT SURFACE: %d\n", current_surface);
	printf("AB(RED): %d BC(GREEN): %d CA(BLUE): %d\n", surfaces[current_surface].ab, surfaces[current_surface].bc, surfaces[current_surface].ca);
	
	return true;
}
