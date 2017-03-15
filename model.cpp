#include "model.h"

Model::Model() {
	points_count = 0;
	normals_count = 0;
	triangles_count = 0;
}

Model::Model(char* data, int length) {
	this->load_data(data, length);
}

void Model::load_data(char* data, int length) {
	points_count = 0;
	normals_count = 0;
	triangles_count = 0;
	
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
					
			case 'f':
				triangles_count++; break;
		}
		
		i = next_line(data, i);
	}
	
	points = new Vector[points_count];
	normals = new Vector[normals_count];
	triangles = new Triangle[triangles_count];
	
	points_count = 0;
	normals_count = 0;
	triangles_count = 0;
	
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
					
			case 'f':
				Triangle t;
				int a_p_start = next_non_whitespace(data, i);
				t.a = atoi(data+a_p_start) - 1;
				int b_p_start = next_non_whitespace(data, a_p_start);
				t.b = atoi(data+b_p_start) - 1;
				int c_p_start = next_non_whitespace(data, b_p_start);
				t.c = atoi(data+c_p_start) - 1;
				
				int n_start = next_numeric(data, c_p_start);
				t.normal = atoi(data+n_start) - 1;
				
				triangles[triangles_count] = t;
				triangles_count++; 
				break;
		}
		
		i = next_line(data, i);
	}
	
	//printf("POINTS: %d\nNORMALS: %d\nTRIANGLES: %d\n", points_count, normals_count, triangles_count);
}