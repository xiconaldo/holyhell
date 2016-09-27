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
	std::vector<Vertex> data;
	load_grouped_data(base_data_location + object_name, triangle_count, data);
	data.pop_back();
	data.pop_back();

	std::cout << base_data_location + object_name << std::endl;
	std::cout << base_text_location + text_name << std::endl;

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE1);
	ktxLoadTextureN((base_text_location + text_name).c_str(), &text_map, NULL, NULL, NULL, NULL, NULL, NULL);
}

void Terrain::draw(){
	glDrawArrays(GL_PATCHES, 0, 3*triangle_count);
}

void Terrain::setBaseTextLocation(const std::string& location){
	base_text_location = location;
}

std::string Terrain::base_text_location = "";