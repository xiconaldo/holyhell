#include <scene.h>

/**
 * Contrutor vazio.
 */
Terrain::Terrain(){}

Terrain::~Terrain(){
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteSamplers(1, &samp);
	glDeleteTextures(1, &text_gl_name);
	glDeleteTextures(1, &text_map);
}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Terrain::loadData(const std::string& object_name, const std::string& text_name, const std::string& text_map_name){
	
	load_grouped_data(base_data_location + object_name, triangle_count, data, false);

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

	glGenTextures(1, &text_map);
	error = ktxLoadTextureN((base_text_location + text_map_name).c_str(), &text_map, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

void Terrain::draw(){
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(text_target, text_map);

	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, 4);
}