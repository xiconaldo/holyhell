#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ios>
#include <sstream>

typedef struct{
	float x, y, z, w;
} Vertex;

typedef struct{
	Vertex vertices;
	Vertex normals;
	Vertex texts; 
} Group;

int count_symbol(const std::string& word, char symbol = ' ');

void load_data(	const std::string& obj_name,
				Vertex** const vertices,
				Vertex** const normals,
				Vertex** const uvs, 
				int** vertex_indices, 
				int** normal_indices, 
				int** uv_indices,
				int& vertex_count, 
				int& normal_count, 
				int& uv_count,
				int& triangle_count);

void group_data(Vertex* const vertices,
				Vertex* const normals,
				Vertex* const uvs, 
				int* vertex_indices, 
				int* normal_indices, 
				int* uv_indices,
				int triangle_count,
				Group** const data);

void load_grouped_data(	const std::string& obj_name,
						int& triangle_count,
						Group** const data);

std::string load_shader(const std::string& file);

#endif