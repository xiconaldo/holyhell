#include <scene.h>

Object::Object(){}

GLuint Object::loadData(const std::string& object_name){
	std::vector<Vertex> data;
	load_grouped_data(base_data_location + object_name, triangle_count, data);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);
}

void Object::rotate(float degrees, float xc, float yc, float zc){
	glm::vec3 axis(xc, yc, zc);
	axis = glm::normalize(axis);

	rotation = glm::cross(glm::angleAxis(degrees, axis), rotation);
	updateModel();
	updateModelMatrix();
}

void Object::translate(float x, float y, float z){
	//m_model = glm::translate(glm::vec3(x, y, z)) * m_model;
	translation += glm::vec3(x, y, z);
	updateModel();
	updateModelMatrix();
}

void Object::scale(float x, float y, float z){
	//m_model = glm::scale(glm::vec3(x, y, z)) * m_model;
	scalation *= glm::vec3(x, y, z);
	updateModel();
	updateModelMatrix();
}

void Object::makeActiveOnProgram(GLuint program){

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	vertex_location = glGetAttribLocation(program, "vertex");
	normal_location = glGetAttribLocation(program, "normal");
	model_location = glGetUniformLocation(program, "model");

	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));
	glEnableVertexAttribArray(vertex_location);

	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(normal_location);

	updateModelMatrix();
}

void Object::makeActiveOnLocation(GLuint vertexLocation, GLuint normalLocation, GLuint modelLocation){

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	vertex_location = vertexLocation;
	normal_location = normalLocation;
	model_location = modelLocation;

	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));
	glEnableVertexAttribArray(vertex_location);

	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(normal_location);

	updateModelMatrix();
}

void Object::draw(){
	glDrawArrays(GL_TRIANGLES, 0, 3*triangle_count);
}

void Object::setBaseDataLocation(const std::string& location){
	base_data_location = location;
}

void Object::updateModel(){
	m_model = glm::translate(translation) *
			  glm::mat4_cast(rotation) *
			  glm::scale(scalation);
}

void Object::updateModelMatrix(){
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
}

void Object::resetModel(){
	translation = glm::vec3(0.0f);
	rotation = glm::quat(0.0f, 1.0f, 0.0f, 0.0f);
	scalation = glm::vec3(1.0f);
}

std::string Object::base_data_location = "";