#include <scene.h>

Object::Object(){}

void Object::load_data(){
	load_grouped_data(data_location, triangles, coord);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, coord.size() * sizeof(Vertex), (void*)coord.data(), GL_STATIC_DRAW);
}

void Object::draw(){
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3*triangles);
}