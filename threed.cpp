#include "threed.h"

ThreeD::ThreeD() {

}

void ThreeD::init(Matrix p, Matrix v, double render_w, double render_h) {
	p_matrix = p;
	v_matrix = v;
	pv_matrix = (p_matrix * v_matrix);

	this->render_w = render_w;
	this->render_h = render_h;
}

void ThreeD::update_v_matrix(Matrix v) {
	v_matrix = v;
	pv_matrix = v_matrix * p_matrix;
}

void ThreeD::update_p_matrix(Matrix p) {
	p_matrix = p;
	pv_matrix = v_matrix * p_matrix;
}

Vector ThreeD::get_projected(Vector a) {
	Matrix m(a);
	m *= pv_matrix;
	Vector p_v = m.get_vector();
	
	if (m.x[3] != 1) {
		p_v /= m.x[3];
	}
	
	p_v.x = dmin(SCREEN_W - 1, (p_v.x + 1) * 0.5 * SCREEN_W);
	p_v.y = dmin(SCREEN_H - 1,  (1 - (p_v.y + 1) * 0.5) * SCREEN_H);
	
	return p_v;
}

void ThreeD::draw_point(Vector a, uint8_t c) {
	if (((int)a.y>=SCREEN_H) || ((int)a.y<0) || ((int)a.x>=SCREEN_W) || ((int)a.x<0))
		return;
	
	unsigned int offset = a.y*SCREEN_W+a.x;
	if (a.z < depth_buffer[offset]) {
		set_pixel(a.x, a.y, c);
		depth_buffer[offset] = a.z;
	}
}

void ThreeD::draw_point_3d(Vector a, uint8_t c) {
	Vector p_v = get_projected(a);
	draw_point(p_v, c);
}

void ThreeD::draw_line(Vector pa, Vector pb, uint8_t c) {
	if (pa.x > pb.x) swap(&pa, &pb);
	for (int x=(int)pa.x; x<=(int)pb.x; x++) {
		double z = interpolate(pa.z, pb.z, (x-pa.x)/(pb.x-pa.x));
		draw_point((Vector){x, pa.y, z}, c);
	}
}

void ThreeD::draw_line_3d(Vector pa, Vector pb, uint8_t c) {
	Vector p_pa = get_projected(pa);
	Vector p_pb = get_projected(pb);
	
	line(p_pa.x, p_pa.y, p_pb.x, p_pb.y, c);
}

void ThreeD::draw_triangle(Vector p1, Vector p2, Vector p3, uint8_t c) {
	if ((int)p1.y==(int)p2.y && (int)p1.y==(int)p3.y) return;
	if ((int)p1.x==(int)p2.x && (int)p1.x==(int)p3.x) return;
	
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
}

void ThreeD::tft(Vector pa, Vector pb, Vector pc, uint8_t c) {
	for (int y=(int)pc.y; y<=(int)pa.y; y++) {
		double gradient = (y-(int)pc.y)/(double)((int)pa.y-(int)pc.y);
		double x1 = interpolate(pc.x, pa.x, gradient);
		double x2 = interpolate(pc.x, pb.x, gradient);
		double z1 = interpolate(pc.z, pa.z, gradient);
		double z2 = interpolate(pc.z, pb.z, gradient);
		draw_line((Vector){x1, y, z1}, (Vector){x2, y, z2}, c);
	}
}

void ThreeD::bft(Vector pa, Vector pb, Vector pc, uint8_t c) {
	for (int y=(int)pa.y; y>=(int)pb.y; y--) {
		double gradient = ((int)pa.y-y)/(double)((int)pa.y-(int)pb.y);
		double x1 = interpolate(pa.x, pb.x, gradient);
		double x2 = interpolate(pa.x, pc.x, gradient);
		double z1 = interpolate(pa.z, pb.z, gradient);
		double z2 = interpolate(pa.z, pc.z, gradient);
		draw_line((Vector){x1, y, z1}, (Vector){x2, y, z2}, c);
	}
}

void ThreeD::draw_model_3d(const Model& m, uint8_t c) {
	Vector points_p[m.points_count];
	for (int i=0; i < m.points_count; i++) {
		points_p[i] = get_projected(m.points[i]);
	}
	/*
	// todo get angle to camera so we can skip certain triangles
	double camera_angles[m.normals_count];
	for (int i=0; i < m.normals_count; i++) {
		Matrix normal(m.normals[i]);
		
	}
	*/
	
	for (int i=0; i < m.triangles_count; i++) {
		draw_triangle(points_p[m.triangles[i].a], points_p[m.triangles[i].b], points_p[m.triangles[i].c], c);
	}
}



void ThreeD::clear_depth_buffer() {
	for (int d=0; d<SCREEN_W*SCREEN_H; d++) {
		depth_buffer[d] = 100.0;
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