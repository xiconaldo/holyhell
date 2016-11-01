#include <scene.h>

/**
 * Contrutor vazio.
 */
Grass::Grass(){}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Grass::loadData(const std::string& object_name, const std::string& text_name){
	
	load_grouped_data(base_data_location + object_name, triangle_count, data, true);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);

	glGenSamplers(1, &samp);
	glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindSampler(text_target, samp);

	glActiveTexture(GL_TEXTURE0);
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

void Grass::draw(){

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);

	glBindVertexArray(vao);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3*triangle_count, 512*512);

}
