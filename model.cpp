#include "model.h"

Model::Model() {
	points_count = 0;
	normals_count = 0;
	triangles_count = 0;
	
	double identity_x[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
	Matrix identity(4, 4, identity_x);
	transform = identity;
}

Model::Model(char* data, int length) {
	this->load_data(data, length);
}

void Model::load_data(char* data, int length) {
	points_count = 0;
	normals_count = 0;
	triangles_count = 0;
	materials_count = 0;
	
	double identity_x[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
	Matrix identity(4, 4, identity_x);
	transform = identity;
	
	int i=0;
	while (i<length) {
		switch (data[i]) {
			case 'v': 
				if (data[i+1] == 'n') {
					normals_count++; break;
				} else if (is_whitespace(data[i+1])) {
					points_count++; break;
				} else {
					break;
				}
			case 'u':
				if (starts_with(data+i, "usemtl")) {
					materials_count++;
				} else {
					break;
				}
					
			case 'f':
				triangles_count++; break;
		}
		
		i = next_line(data, i);
	}
	
	points = new Vector[points_count];
	normals = new Vector[normals_count];
	triangles = new Triangle[triangles_count];
	materials = new Material[materials_count];
	
	points_count = 0;
	normals_count = 0;
	triangles_count = 0;
	materials_count = 0;
	
	int current_material = -1;
	
	i=0;
	while (i<length) {
		switch (data[i]) {
			case 'v': 
				if (data[i+1] == 'n') {
					Vector v;
					int x_start = next_non_whitespace(data, i);
					v.x = atod(data+x_start);
					int y_start = next_non_whitespace(data, x_start);
					v.y = atod(data+y_start);
					int z_start = next_non_whitespace(data, y_start);
					v.z = atod(data+z_start);
					
					v *= -1;
					
					normals[normals_count] = v;
					normals_count++; 
					break;
				} else if (is_whitespace(data[i+1])) {
					Vector v;
					int x_start = next_non_whitespace(data, i);
					v.x = atod(data+x_start);
					int y_start = next_non_whitespace(data, x_start);
					v.y = atod(data+y_start);
					int z_start = next_non_whitespace(data, y_start);
					v.z = atod(data+z_start);
					
					points[points_count] = v;
					points_count++; 
					break;
				} else {
					break;
				}
			
			case 'u':
				if (starts_with(data+i, "usemtl")) {
					int name_start = next_whitespace(data, i) + 1;
					int name_end = next_whitespace(data, name_start);
					int name_length = name_end-name_start;
					
					Material m;
					m.name = new char[name_length+1];
					memset(m.name, 0, name_length+1);
					memcpy(m.name, data+name_start, name_length);
					
					if (get_material(m.name) == -1) {
						m.color = 0xff00ff;
						materials[materials_count] = m;
						current_material = materials_count;
						materials_count++;
					}
					break;
				} else {
					break;
				}
					
			case 'f':
				Triangle t;
				int a_p_start = next_non_whitespace(data, i);
				t.a = atoi(data+a_p_start) - 1;
				int b_p_start = next_non_whitespace(data, a_p_start);
				t.b = atoi(data+b_p_start) - 1;
				int c_p_start = next_non_whitespace(data, b_p_start);
				t.c = atoi(data+c_p_start) - 1;

				int n_start = next_char(data, '/', c_p_start);
				n_start = next_char(data, '/', n_start + 1) + 1;
				t.normal = atoi(data+n_start) - 1;
				
				t.mtl = current_material;
				
				triangles[triangles_count] = t;
				triangles_count++; 
				break;
		}
		
		i = next_line(data, i);
	}
	
	//printf("POINTS: %d\nNORMALS: %d\nTRIANGLES: %d\nMATERIALS: %d\n", points_count, normals_count, triangles_count, materials_count);
}

int Model::get_material(char* name) {
	for (int i=0; i < materials_count; i++) {
		if (starts_with(materials[i].name, name))
			return i;
	}
	return -1;
}

void Model::print() {
	printf("POINTS:\n");
	for (int i=0; i<points_count; i++) {
		points[i].print();
	}
	printf("NORMALS:\n");
	for (int i=0; i<normals_count; i++) {
		normals[i].print();
	}
	printf("TRIANGLES:\n");
	for (int i=0; i<triangles_count; i++) {
		printf("A: %d B: %d C: %d N: %d\n", triangles[i].a, triangles[i].b, triangles[i].c, triangles[i].normal);
	}
	printf("MATERIALS:\n");
	for (int i=0; i<materials_count; i++) {
		printf("NAME: %s C: %#08x\n", materials[i].name, materials[i].color);
	}
	
	printf("POINTS: %d NORMALS: %d TRIANGLES: %d\n", points_count, normals_count, triangles_count);
}

Model create_face(Vector a, Vector b, Vector c, Vector d, uint32_t color) {
	Model m;
	m.points_count = 4;
	m.normals_count = 1;
	m.triangles_count = 2;
	m.materials_count = 1;
	
	m.points = new Vector[m.points_count];
	m.points[0] = a;
	m.points[1] = b;
	m.points[2] = c;
	m.points[3] = d;
	
	m.normals = new Vector[m.normals_count];
	m.normals[0] = calculate_surface_normal(a, b, c);
	
	m.triangles = new Triangle[m.triangles_count];
	m.triangles[0] = (Triangle) {0, 1, 2, 0, 0};
	m.triangles[1] = (Triangle) {0, 2, 3, 0, 0};
	
	m.materials = new Material[1];
	m.materials[0] = (Material) {"default", color};
	
	return m;
}

Model create_face(Vector pos, double width, double height, Vector up, uint32_t color) {
	Vector a = (Vector){0,0,0};
	Vector b = (Vector){0, height, 0};
	Vector c = (Vector){width, height, 0};
	Vector d = (Vector){width, 0, 0};
	
	if (!(up.x == 0 && up.y == 0 && up.z == 1)) {
		Vector axis = up.cross((Vector){0,0,1});
		double cos_angle = up.dot((Vector){0,0,1});
		Matrix transform = IDENTITY.rotated_3d(axis, cos_angle+1, cos_angle);
		a = (transform * Matrix(a)).get_vector();
		b = (transform * Matrix(b)).get_vector();
		c = (transform * Matrix(c)).get_vector();
		d = (transform * Matrix(d)).get_vector();
	}
	
	a += pos;
	b += pos;
	c += pos;
	d += pos;
	
	return create_face(a, b, c, d, color);
}

Model create_box(Vector c, double lx, double ly, double lz, uint32_t color) {
	Model m;
	m.points_count = 8;
	m.normals_count = 6;
	m.triangles_count = 12;
	m.materials_count = 1;
	
	double lx2 = lx/2.0;
	double ly2 = ly/2.0;
	double lz2 = lz/2.0;
	
	m.points = new Vector[m.points_count];
	m.points[0] = (Vector){c.x - lx2, c.y + ly2, c.z - lz2};
	m.points[1] = (Vector){c.x + lx2, c.y + ly2, c.z - lz2};
	m.points[2] = (Vector){c.x + lx2, c.y - ly2, c.z - lz2};
	m.points[3] = (Vector){c.x - lx2, c.y - ly2, c.z - lz2};
	
	m.points[4] = (Vector){c.x - lx2, c.y + ly2, c.z + lz2};
	m.points[5] = (Vector){c.x + lx2, c.y + ly2, c.z + lz2};
	m.points[6] = (Vector){c.x + lx2, c.y - ly2, c.z + lz2};
	m.points[7] = (Vector){c.x - lx2, c.y - ly2, c.z + lz2};
	
	m.normals = new Vector[m.normals_count];
	m.normals[0] = calculate_surface_normal(m.points[2], m.points[1], m.points[0]);
	m.normals[1] = calculate_surface_normal(m.points[0], m.points[1], m.points[2]);
	
	m.normals[2] = calculate_surface_normal(m.points[0], m.points[1], m.points[5]);
	m.normals[3] = calculate_surface_normal(m.points[5], m.points[1], m.points[0]);
	
	m.normals[4] = calculate_surface_normal(m.points[3], m.points[0], m.points[4]);
	m.normals[5] = calculate_surface_normal(m.points[4], m.points[0], m.points[3]);
	
	m.triangles = new Triangle[m.triangles_count];
	m.triangles[0] = (Triangle){0, 1, 2, 0, 0};
	m.triangles[1] = (Triangle){0, 2, 3, 0, 0};

	m.triangles[2] = (Triangle){4, 5, 6, 1, 0};
	m.triangles[3] = (Triangle){4, 6, 7, 1, 0};

	m.triangles[4] = (Triangle){0, 1, 5, 2, 0};
	m.triangles[5] = (Triangle){0, 5, 4, 2, 0};

	m.triangles[6] = (Triangle){2, 3, 7, 3, 0};
	m.triangles[7] = (Triangle){2, 7, 6, 3, 0};

	m.triangles[8] = (Triangle){3, 0, 4, 4, 0};
	m.triangles[9] = (Triangle){3, 4, 7, 4, 0};
	
	m.triangles[10] = (Triangle){1, 2, 6, 5, 0};
	m.triangles[11] = (Triangle){1, 6, 5, 5, 0};

	m.materials = new Material[1];
	m.materials[0] = (Material) {"default", color};
	
	return m;
}