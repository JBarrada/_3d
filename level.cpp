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
	player_step_size = 0.2;
}

Level::Level(Model m) {
	this->m = m;
	
	current_surface = 0;
	surface_pos = (Vector){0,0,0};
	world_pos = (Vector){0,0,0};
	world_up = (Vector){0,0,1};
	
	player_dir = 0.0;
	player_step_size = 0.2;
	
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
			//Vector test_n = (Vector)m.normals[n];
			//test_n *= -1;
			//Vector axis = test_n.cross((Vector){0,0,1});
			//double cos_angle = test_n.dot((Vector){0,0,1});
			//this->s_to_n[n] = this->s_to_n[n].rotated_3d(axis, cos_angle, cos_angle+1);
			/*
			Vector axis = m.normals[n].cross((Vector){0,0,1});
			double cos_angle = m.normals[n].dot((Vector){0,0,1});
			this->s_to_n[n] = this->s_to_n[n].rotated_3d(axis, cos_angle+1, cos_angle);
			*/
		}
	}
	
	this->n_to_s = new Matrix[m.normals_count];
	
	/*
	Vector up(0,0,1);
	for (int n=0; n<m.normals_count; n++) {
		this->n_to_s[n] = (Matrix)IDENTITY;
		if (!(m.normals[n].x == 0 && m.normals[n].y == 0 && m.normals[n].z == 1) && !(m.normals[n].x == 0 && m.normals[n].y == 0 && m.normals[n].z == -1)) {
			Vector axis = up.cross(m.normals[n]) * -1;
			double cos_angle = up.dot(m.normals[n]) * -1;
			
			printf("NORMAL : ");
			m.normals[n].print();
			
			printf("AXIS : ");
			axis.print();
			
			printf("COS X : %f\n", cos_angle);
			printf("\n");
			
			this->n_to_s[n] = this->n_to_s[n].rotated_3d(axis, cos_angle+1, cos_angle);
			
			//debug
			this->s_to_n[n] = this->s_to_n[n].rotated_3d(axis * -1, cos_angle+1, cos_angle);
		}
	}
	*/
	
	// maybe better https://www.gamedev.net/topic/61727-aligning-two-vectors/
	for (int n=0; n<m.normals_count; n++) {
		this->n_to_s[n] = (Matrix)IDENTITY;
		
		Vector axis(m.normals[n].y, -m.normals[n].x, 0);
		double angle = m.normals.z;

		this->n_to_s[n] = this->n_to_s[n].rotated_3d(axis, cos_angle+1, cos_angle);
	}
	
}

Vector Level::transform_n_to_s(Vector& v, int s) {
	Vector v_transformed = (this->n_to_s[m.triangles[s].normal] * Matrix(v)).get_vector();
	v_transformed.z = 0.0;
	return v_transformed;
}

Vector Level::transform_s_to_n(Vector& v, int s) {
	return (this->s_to_n[m.triangles[s].normal] * Matrix(v)).get_vector();
}

float tri_sign(Vector& p1, Vector& p2, Vector& p3) {
  return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Level::move(bool forward) {
	Vector step(cos(player_dir)*0.2, sin(player_dir)*0.2, 0);
	if (!forward) {
		step *= -1;
	}
	Vector test_pos = (Vector)surface_pos + step;
	
	Vector a1 = transform_n_to_s(m.points[m.triangles[current_surface].a], current_surface);
	Vector b1 = transform_n_to_s(m.points[m.triangles[current_surface].b], current_surface);
	Vector c1 = transform_n_to_s(m.points[m.triangles[current_surface].c], current_surface);

	bool ab_sign = tri_sign(test_pos, a1, b1) < 0.0;
	bool bc_sign = tri_sign(test_pos, b1, c1) < 0.0;
	bool ca_sign = tri_sign(test_pos, c1, a1) < 0.0;
	bool majority_sign = (ab_sign + bc_sign + ca_sign) > 1;
	
	if ((ab_sign == bc_sign) && (bc_sign == ca_sign)) {
		Vector s_to_n_offset = (Vector)m.points[m.triangles[current_surface].a] - transform_s_to_n(a1, current_surface);
		//printf("S TO N OFFSET: ");
		//s_to_n_offset.print();
		
		surface_pos = (Vector)test_pos;
		world_pos = transform_s_to_n(surface_pos, current_surface) + s_to_n_offset;
		world_up = (Vector)m.normals[m.triangles[current_surface].normal];
		
		//surface_pos.print();
		//world_pos.print();
		return false;
		
	} else {
		Vector edge1;
		Vector edge2;
		Vector edge2_anchor;
		Vector i;
		double edge_i_length;
		
		Vector s_to_n_offset;
		
		if (ab_sign != majority_sign) {
			if (surfaces[current_surface].ab == -1) {
				return false;
			} else {
				// next surface
				int next_surface = surfaces[current_surface].ab;
				Vector a2 = transform_n_to_s(m.points[m.triangles[current_surface].a], next_surface);
				Vector b2 = transform_n_to_s(m.points[m.triangles[current_surface].b], next_surface);
				s_to_n_offset = (Vector)m.points[m.triangles[current_surface].a] - transform_s_to_n(a2, next_surface);
				current_surface = next_surface;
				
				edge2_anchor = (Vector)a2;
				
				// find intersection point
				i = intersect(surface_pos, test_pos, a1, b1);
				edge_i_length = (i - a1).length();
				
				edge1 = b1 - a1;
				edge2 = b2 - a2;
				
				printf("AB\n");
			}
		}else if (bc_sign != majority_sign) {
			if (surfaces[current_surface].bc == -1) {
				return false;
			} else {
				// next surface
				int next_surface = surfaces[current_surface].bc;
				Vector b2 = transform_n_to_s(m.points[m.triangles[current_surface].b], next_surface);
				Vector c2 = transform_n_to_s(m.points[m.triangles[current_surface].c], next_surface);
				s_to_n_offset = (Vector)m.points[m.triangles[current_surface].b] - transform_s_to_n(b2, next_surface);
				current_surface = next_surface;
				
				edge2_anchor = (Vector)b2;
				
				// find intersection point
				i = intersect(surface_pos, test_pos, b1, c1);
				edge_i_length = (i - b1).length();
				
				edge1 = c1 - b1;
				edge2 = c2 - b2;
				printf("BC\n");
			}
		}else if (ca_sign != majority_sign) {
			if (surfaces[current_surface].ca == -1) {
				return false;
			} else {
				// next surface
				int next_surface = surfaces[current_surface].ca;
				Vector c2 = transform_n_to_s(m.points[m.triangles[current_surface].c], next_surface);
				Vector a2 = transform_n_to_s(m.points[m.triangles[current_surface].a], next_surface);
				s_to_n_offset = (Vector)m.points[m.triangles[current_surface].c] - transform_s_to_n(c2, next_surface);
				current_surface = next_surface;
				
				edge2_anchor = (Vector)c2;
				
				// find intersection point
				i = intersect(surface_pos, test_pos, c1, a1);
				edge_i_length = (i - c1).length();
				
				edge1 = a1 - c1;
				edge2 = a2 - c2;
				printf("CA\n");
			}
		}
		
		//printf("S TO N OFFSET: ");
		//s_to_n_offset.print();
		
		double edge1_angle = atan2(edge1.y, edge1.x);
		double edge2_angle = atan2(edge2.y, edge2.x);
		double delta_angle = edge2_angle - edge1_angle;

		Vector i1 = edge2_anchor + (((Vector){cos(edge2_angle), sin(edge2_angle),0}) * edge_i_length);

		double l2 = step.length() - (i - surface_pos).length();

		player_dir += delta_angle;
		Vector e = i1 + (((Vector){cos(player_dir), sin(player_dir),0}) * l2);
		
		surface_pos = (Vector)e;
		world_pos = transform_s_to_n(surface_pos, current_surface) + s_to_n_offset;
		world_up = (Vector)m.normals[m.triangles[current_surface].normal];
		
		edge1.print();
		edge2.print();
		
		i.print();
		printf("Edgeilen: %f    edge1ang: %f    edge2ang: %f   delta_angle: %f\n", edge_i_length, edge1_angle, edge2_angle, delta_angle);
		i1.print();
	
		surface_pos.print();
		world_pos.print();
		return true;
	}
}

/*
bool Level::move(Vector& step) {
	Vector test_pos = (Vector)surface_pos + step;
	
	Vector a = (Vector)m.points[m.triangles[current_surface].a];
	Vector b = (Vector)m.points[m.triangles[current_surface].b];
	Vector c = (Vector)m.points[m.triangles[current_surface].c];
	
	// normal transform to surface space
	a = (this->n_to_s[m.triangles[current_surface].normal] * Matrix(a)).get_vector();
	b = (this->n_to_s[m.triangles[current_surface].normal] * Matrix(b)).get_vector();
	c = (this->n_to_s[m.triangles[current_surface].normal] * Matrix(c)).get_vector();
	
	a.z = 0.0;
	b.z = 0.0;
	c.z = 0.0;
	
	
	Vector a_inverse = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(a)).get_vector();
	Vector b_inverse = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(b)).get_vector();
	Vector c_inverse = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(c)).get_vector();
	Vector a_offset = (Vector)m.points[m.triangles[current_surface].a] - a_inverse;
	Vector b_offset = (Vector)m.points[m.triangles[current_surface].b] - b_inverse;
	Vector c_offset = (Vector)m.points[m.triangles[current_surface].c] - c_inverse;
	
	if (!(a_offset == (Vector){0,0,0}) || !(b_offset == (Vector){0,0,0}) || !(c_offset == (Vector){0,0,0})) {
		printf("OFFSET_ISSUE:\n");
		a_offset.print();
		b_offset.print();
		c_offset.print();
		printf("-------------\n");
	}
	
	bool ab_sign = tri_sign(test_pos, a, b) < 0.0;
	bool bc_sign = tri_sign(test_pos, b, c) < 0.0;
	bool ca_sign = tri_sign(test_pos, c, a) < 0.0;
	
	bool majority_sign = (ab_sign + bc_sign + ca_sign) > 1;
	
	if ((ab_sign == bc_sign) && (bc_sign == ca_sign)) {
		this->surface_pos.x = test_pos.x;
		this->surface_pos.y = test_pos.y;
		
		this->world_pos = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(this->surface_pos)).get_vector() + a_offset;
		this->world_up = (Vector)m.normals[m.triangles[current_surface].normal];
		return false;
	} else {
		if (ab_sign != majority_sign) {
			if (surfaces[current_surface].ab == -1) {
				return false;
			} else {
				Vector a_new = (Vector)m.points[m.triangles[current_surface].a];
				a_new = (this->n_to_s[m.triangles[surfaces[current_surface].ab].normal] * Matrix(a_new)).get_vector();
				a_new.z = 0.0;


				current_surface = surfaces[current_surface].ab;
						
				test_pos -= (a - a_new);
				this->surface_pos -= (a - a_new);
				return move(step);
			}
		}else if (bc_sign != majority_sign) {
			if (surfaces[current_surface].bc == -1) {
				return false;
			} else {
				Vector b_new = (Vector)m.points[m.triangles[current_surface].b];
				b_new = (this->n_to_s[m.triangles[surfaces[current_surface].bc].normal] * Matrix(b_new)).get_vector();
				b_new.z = 0.0;


				current_surface = surfaces[current_surface].bc;				
				
				test_pos -= (b - b_new);
				this->surface_pos -= (b - b_new);
				return move(step);
			}
		}else if (ca_sign != majority_sign) {
			if (surfaces[current_surface].ca == -1) {
				return false;
			} else {
				Vector a_new = (Vector)m.points[m.triangles[current_surface].a];
				a_new = (this->n_to_s[m.triangles[surfaces[current_surface].ca].normal] * Matrix(a_new)).get_vector();
				a_new.z = 0.0;


				current_surface = surfaces[current_surface].ca;				
				
				test_pos -= (a - a_new);
				this->surface_pos -= (a - a_new);
				return move(step);
			}
		}
	}
	
	//printf("ab: %d bc: %d bc: %d \n", ab_sign, bc_sign, ca_sign);
	
	this->surface_pos.x = test_pos.x;
	this->surface_pos.y = test_pos.y;
	
	this->world_pos = (this->s_to_n[m.triangles[current_surface].normal] * Matrix(this->surface_pos)).get_vector() + a_offset;
	this->world_up = (Vector)m.normals[m.triangles[current_surface].normal];
	
	surface_pos.print();
	//world_pos.print();
	printf("CURRENT SURFACE: %d\n", current_surface);
	printf("AB(RED): %d BC(GREEN): %d CA(BLUE): %d\n", surfaces[current_surface].ab, surfaces[current_surface].bc, surfaces[current_surface].ca);
	
	return true;
}
*/