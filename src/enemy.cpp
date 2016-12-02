#include <scene.h>

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Enemy::loadData(const std::string& object_name, const std::string& text_name){

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

	glGenTextures(1, &text_gl_name);
	
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

/**
 * Contrutor vazio.
 */
Enemy::Enemy(){}

void Enemy::draw(float x, float z, float factor, float *dist){
	
	factor *= 0.02f;
	glm::vec2 dir(m_model[3].x - x, m_model[3].z - z);
	*dist = glm::length(dir);
	if(*dist > 0.001f){
		dir = glm::normalize(dir) * factor;
		m_model[3].x -= dir.x;
		m_model[3].z -= dir.y;
	}
	
	Object::draw();

}