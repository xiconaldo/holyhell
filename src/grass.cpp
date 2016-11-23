#include <scene.h>

/**
 * Contrutor vazio.
 */
Grass::Grass(){}

Grass::~Grass(){
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteSamplers(1, &samp);
	glDeleteTextures(1, &text_gl_name);
}

void Grass::draw(){
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);

	glBindVertexArray(vao);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3*triangle_count, 512*512);

}
