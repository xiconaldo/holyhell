#include <scene.h>

/**
 * Contrutor vazio.
 */
Grass::Grass(){}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Grass::loadData(const std::string& object_name){
	
	load_grouped_data(base_data_location + object_name, triangle_count, data);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);

}

void Grass::draw(){
	glBindVertexArray(vao);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3*triangle_count, 256*256);
}
