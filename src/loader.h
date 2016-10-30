#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ios>
#include <sstream>
#include <glm/glm.hpp>
#include <vector>

typedef struct{
	glm::vec3 vertices;
	glm::vec3 normals;
	glm::vec2 texts; 
} Vertex;

class Face{
public:
	Face(int n_vertices = 3);
	~Face();

	int vertex_count;
	Vertex *vertices;
};

class Indexed_Face{
public:
	Indexed_Face(int n_vertices = 3);
	~Indexed_Face();
	
	int vertex_count;
	int *vertices;
	int *normals;
	int *texts;
};

int count_symbol(const std::string& word, char symbol = ' ');

void load_data(	const std::string& obj_name,
				std::vector<glm::vec3> &vertices,
				std::vector<glm::vec3> &normals,
				std::vector<glm::vec2> &uvs, 
				std::vector<Indexed_Face> &faces,
				int& vertex_count, 
				int& normal_count, 
				int& uv_count,
				int& face_count);

void group_data(const std::vector<glm::vec3> &vertices,
				const std::vector<glm::vec3> &normals,
				const std::vector<glm::vec2> &uvs, 
				const std::vector<Indexed_Face> &faces,
				std::vector<Vertex> &data,
				const bool triangulate);

void load_grouped_data(	const std::string& obj_name,
						int& face_count,
						std::vector<Vertex> &data,
						const bool triangulate = true);

std::string load_shader(const std::string& file);

#endif