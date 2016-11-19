#include <scene.h>

/**
 * Contrutor vazio.
 */
Grass::Grass(){}

void Grass::draw(){
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);

	glBindVertexArray(vao);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
	glUniformMatrix4fv(rev_model_location, 1, GL_FALSE, glm::value_ptr(reverse_model));
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3*triangle_count, 512*512);

}
