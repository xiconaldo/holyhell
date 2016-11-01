#include <scene.h>

/**
 * Contrutor vazio.
 */
Terrain::Terrain(){}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Terrain::loadData(const std::string& object_name, const std::string& text_name){
	
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

	glActiveTexture(GL_TEXTURE0);
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + "dirt.ktx").c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);

	glActiveTexture(GL_TEXTURE1);
	error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_map, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

void Terrain::draw(){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(text_target, text_map);

	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, 4);
}