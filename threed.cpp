#include "threed.h"

ThreeD::ThreeD() {

}

void ThreeD::init(Matrix p, Matrix v, double render_w, double render_h) {
	p_matrix = p;
	v_matrix = v;
	pv_matrix = (v_matrix * p_matrix);

	this->render_w = render_w;
	this->render_h = render_h;
	
	backface_cull = true;
}

void ThreeD::update_v_matrix(Matrix v) {
	v_matrix = v;
	pv_matrix = (v * p_matrix);
}

void ThreeD::update_p_matrix(Matrix p) {
	p_matrix = p;
	pv_matrix = (v_matrix * p);
}

bool ThreeD::on_screen(const Vector& v) {
	
	if ((v.x >= 0 && v.x < SCREEN_W) && (v.y >= 0 && v.y < SCREEN_H))
		return true;
	return false;
}

Vector ThreeD::get_projected(Vector a) {
	Matrix m(a);
	Matrix m_v = m * pv_matrix;
	Vector p_v = m_v.get_vector();
	
	if (m_v.x[3] != 1) {
		p_v /= m_v.x[3];
	}

	p_v.x = (p_v.x + 1) * 0.5 * SCREEN_W;
	p_v.y = (1 - (p_v.y + 1) * 0.5) * SCREEN_H;

	return p_v;
}

void ThreeD::draw_point(Vector a, uint32_t c) {
	if (((int)a.y>=SCREEN_H) || ((int)a.y<0) || ((int)a.x>=SCREEN_W) || ((int)a.x<0))
		return;
	
	unsigned int offset = a.y*SCREEN_W+a.x;
	if (a.z > depth_buffer[offset]) {
		set_pixel_32bit(a.x, a.y, c);
		depth_buffer[offset] = a.z;
	}
}

void ThreeD::draw_point_3d(Vector a, uint32_t c) {
	Vector p_v = get_projected(a);
	draw_point(p_v, c);
}

void ThreeD::draw_line(Vector pa, Vector pb, uint32_t c) {
	if (pa.x > pb.x) swap(&pa, &pb);
	
	pa.x = round(pa.x);
	pb.x = round(pb.x);
	pa.y = round(pa.y);
	
	if (pa.x == pb.x) {
		double z = (pa.z + pb.z) / 2.0;
		draw_point((Vector){pa.x, pa.y, z}, c);
	} else {
		for (int x=dmax(pa.x, 0); x<=dmin(pb.x, SCREEN_W); x++) {
			double z = interpolate(pa.z, pb.z, (x-pa.x)/(pb.x-pa.x));
			
			draw_point((Vector){x, pa.y, z}, c);
		}
	}
}

void ThreeD::draw_line_3d(Vector pa, Vector pb, uint8_t c) {
	Vector p_pa = get_projected(pa);
	Vector p_pb = get_projected(pb);
	
	line(p_pa.x, p_pa.y, p_pb.x, p_pb.y, c);
}

bool ThreeD::triangle_on_screen(Vector& p1, Vector& p2, Vector& p3) {
	Vector points[3] = {p1, p2, p3};
	int num_ortho[4] = {0,0,0,0};
			
	for (int p = 0; p < 3; p++) {		
		if (points[p].x < 0) {
			num_ortho[0] += 1;
		}
		if (points[p].x > SCREEN_W) {
			num_ortho[1] += 1;
		}
		if (points[p].y < 0) {
			num_ortho[2] += 1;
		}
		if (points[p].y > SCREEN_H) {
			num_ortho[3] += 1;
		}
	}
	
	return !(num_ortho[0] == 3 || num_ortho[1] == 3 || num_ortho[2] == 3 || num_ortho[3] == 3);
}

void ThreeD::draw_triangle(Vector p1, Vector p2, Vector p3, uint32_t c) {
	if (!triangle_on_screen(p1, p2, p3))
		return;
	
	p1.x = round(p1.x);
	p2.x = round(p2.x);
	p3.x = round(p3.x);
	
	p1.y = round(p1.y);
	p2.y = round(p2.y);
	p3.y = round(p3.y);
	
	if (p1.y==p2.y && p1.y==(int)p3.y) return;
	if (p1.x==p2.x && p1.x==(int)p3.x) return;
	
	if (p3.y > p1.y) swap(&p1, &p3);
	if (p2.y > p1.y) swap(&p1, &p2);
	if (p3.y > p2.y) swap(&p2, &p3);
	
	if (p2.y == p3.y) {
		bft(p1, p2, p3, c);
	} else if (p1.y == p2.y) {
		tft(p1, p2, p3, c);
	} else {
		double xi = interpolate(p3.x, p1.x, (p2.y-p3.y)/(p1.y-p3.y));
		double zi = interpolate(p3.z, p1.z, (p2.y-p3.y)/(p1.y-p3.y));
		bft(p1, (Vector){xi, p2.y, zi}, p2, c);
		tft(p2, (Vector){xi, p2.y, zi}, p3, c);
	}
	
	// toon stuff
	toon_mask_line(p1.x, p1.y, p2.x, p2.y, 5.0);
	toon_mask_line(p1.x, p1.y, p3.x, p3.y, 5.0);
	toon_mask_line(p3.x, p3.y, p2.x, p2.y, 5.0);
}
void ThreeD::tft(Vector pa, Vector pb, Vector pc, uint32_t c) {
	for (int y=dmax(pc.y,0); y<=dmin(pa.y, SCREEN_H); y++) {
		double gradient = (y-pc.y)/(double)(pa.y-pc.y);
		double x1 = interpolate(pc.x, pa.x, gradient);
		double x2 = interpolate(pc.x, pb.x, gradient);
		double z1 = interpolate(pc.z, pa.z, gradient);
		double z2 = interpolate(pc.z, pb.z, gradient);
		draw_line((Vector){x1, y, z1}, (Vector){x2, y, z2}, c);
	}
}
void ThreeD::bft(Vector pa, Vector pb, Vector pc, uint32_t c) {
	pb.y = round(pb.y);
	for (int y=dmin(pa.y, SCREEN_H); y>=dmax(pb.y, 0); y--) {
		double gradient = (pa.y-y)/(double)(pa.y-pb.y);
		double x1 = interpolate(pa.x, pb.x, gradient);
		double x2 = interpolate(pa.x, pc.x, gradient);
		double z1 = interpolate(pa.z, pb.z, gradient);
		double z2 = interpolate(pa.z, pc.z, gradient);
		draw_line((Vector){x1, y, z1}, (Vector){x2, y, z2}, c);
	}
}

Vector ray_plane_intersect(Vector& a, Vector& b, Vector& n, double d) {
	Vector ba = (b - a).norm();
	double n_a = n.dot(a);
	double n_ba = n.dot(ba);
	return a + (ba * ((d - n_a)/n_ba));
}
double point_plane_side(Vector&a, Vector& b, Vector& n) {
	Vector ba = b - a;
	return ba.dot(n);
}

void ThreeD::draw_model_3d(const Model& m) {
	Vector points_t[m.points_count];
	Vector points_p[m.points_count];
	
	Matrix transform_copy = m.transform;
	for (int i=0; i < m.points_count; i++) {
		Matrix point(m.points[i]);
		Matrix translated_point = (transform_copy * point);
		points_t[i] = translated_point.get_vector();
		points_p[i] = get_projected(points_t[i]);
	}

	double camera_angles[m.normals_count];
	for (int i=0; i < m.normals_count; i++) {
		Matrix normal(m.normals[i]);
		Vector transformed_normal = normal.get_vector();
		
		Matrix inv((Vector){0,0,-1});
		Vector camera_r = (v_matrix * inv).get_vector();
		Matrix camera_m(camera_r);
		Vector camera = (camera_m * transform_copy).get_vector();
		
		camera_angles[i] = ((camera.dot(transformed_normal) / (camera.length() * transformed_normal.length()))+1.0)/2.0;
	}
	
	// frustum clipping
	for (int i=0; i < m.triangles_count; i++) {
		if ((camera_angles[m.triangles[i].normal] >= 0.4 && backface_cull) || (!backface_cull) || (!m.backface_cull)) {
			int projected[] = {m.triangles[i].a, m.triangles[i].b, m.triangles[i].c};

			int num_behind = 0;
			int behind = -1;
			int front = -1;
			
			for (int p = 0; p < 3; p++) {
				if (point_plane_side(points_t[projected[p]], this->camera_pos, this->np_n) > 0.0) {
					num_behind++;
					behind = p;
				} else {
					front = p;
				}
			}
			
			uint32_t color = m.materials[m.triangles[i].mtl].color;
			uint32_t i_color = interpolate_color_32bit(color, 1.0 - (dmin(camera_angles[m.triangles[i].normal] - 0.4, 0.5)/1.5));

			if (num_behind == 0) {
				draw_triangle(points_p[m.triangles[i].a], points_p[m.triangles[i].b], points_p[m.triangles[i].c], i_color);
			}
			if (num_behind == 1) {
				Vector c1 = ray_plane_intersect(points_t[projected[(behind+1)%3]], points_t[projected[behind]], this->np_n, this->np_d);
				Vector c2 = ray_plane_intersect(points_t[projected[(behind+2)%3]], points_t[projected[behind]], this->np_n, this->np_d);
				
				Vector c1_p = get_projected(c1);
				Vector c2_p = get_projected(c2);

				draw_triangle(points_p[projected[(behind+1)%3]], points_p[projected[(behind+2)%3]], c1_p, i_color);
				draw_triangle(points_p[projected[(behind+1)%3]], points_p[projected[(behind+2)%3]], c2_p, i_color);
			}
			if (num_behind == 2) {
				Vector b2 = ray_plane_intersect(points_t[projected[front]], points_t[projected[(front+1)%3]], this->np_n, this->np_d);
				Vector c2 = ray_plane_intersect(points_t[projected[front]], points_t[projected[(front+2)%3]], this->np_n, this->np_d);
				
				Vector b2_p = get_projected(b2);
				Vector c2_p = get_projected(c2);
				
				draw_triangle(points_p[projected[front]], b2_p, c2_p, i_color);
			}
		}
	}
}



void ThreeD::clear_depth_buffer() {
	for (int d=0; d<SCREEN_W*SCREEN_H; d++) {
		depth_buffer[d] = 0.0;
	}
}

Matrix look_at_camera(Vector eye, Vector target, Vector up) {
	Vector zaxis = (target - eye).norm();
	Vector xaxis = (up.cross(zaxis)).norm();
	Vector yaxis = zaxis.cross(xaxis);
	
	double m_ox[] = {xaxis.x, yaxis.x, zaxis.x, 0,
					xaxis.y, yaxis.y, zaxis.y, 0,
					xaxis.z, yaxis.z, zaxis.z, 0,
					-1* xaxis.dot(eye), -1* yaxis.dot(eye), -1* zaxis.dot(eye), 1};
	Matrix m_o(4, 4, m_ox);
	

	return m_o;
}

Matrix projection_ortho(double w, double h, double zf, double zn) {
	double m_x[] = {1.0/w, 0, 0, 0,  0, 1.0/h, 0, 0,  0, 0, -2/(zf-zn), -(zf+zn)/(zf-zn),  0, 0, 0, 1};
	Matrix m(4, 4, m_x);
	return m;
}

Matrix projection_persp(double fov_x, double fov_y, double zf, double zn) {
	double m_x[] = {atan(fov_x/2.0), 0, 0, 0,  0, atan(fov_y/2.0), 0, 0,  0, 0,-(zf+zn)/(zf-zn),  (-2.0*zf*zn)/(zf-zn), 0, 0, -1, 0};
	Matrix m(4, 4, m_x);
	return m;
}

Matrix projection_persp_gl(double fov, double zf, double zn, double ratio) {
	double scale = tan(fov * 0.5 * M_PI / 180.0) * zn;
	double r = ratio * scale;
	double l = -r;
	double t = scale;
	double b = -t;
	
	double m_x[] = {2 * zn / (r - l), 0, (r + l) / (r - l), 0,
					0, 2 * zn / (t - b), (t + b) / (t - b), 0,
					0, 0, -(zf + zn) / (zf - zn), -2 * zf * zn / (zf - zn),
					0,0,-1,0};
	Matrix m(4, 4, m_x);
	return m;
}