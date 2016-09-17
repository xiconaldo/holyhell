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

int count_symbol(const std::string& word, char symbol = ' ');

void load_data(	const std::string& obj_name,
				std::vector<glm::vec3> &vertices,
				std::vector<glm::vec3> &normals,
				std::vector<glm::vec2> &uvs, 
				std::vector<int> &vertex_indices, 
				std::vector<int> &normal_indices, 
				std::vector<int> &uv_indices,
				int& vertex_count, 
				int& normal_count, 
				int& uv_count,
				int& triangle_count);

void group_data(const std::vector<glm::vec3> &vertices,
				const std::vector<glm::vec3> &normals,
				const std::vector<glm::vec2> &uvs, 
				const std::vector<int> &vertex_indices, 
				const std::vector<int> &normal_indices, 
				const std::vector<int> &uv_indices,
				const int triangle_count,
				std::vector<Vertex> &data);

void load_grouped_data(	const std::string& obj_name,
						int& triangle_count,
						std::vector<Vertex> &data);

std::string load_shader(const std::string& file);

#endif