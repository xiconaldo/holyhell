#include <loader.h>

Face::Face(int n_vertices){
	vertex_count = n_vertices;
	vertices = new Vertex[vertex_count];
}

Face::~Face(){
	delete[] vertices;
}

Indexed_Face::Indexed_Face(int n_vertices){
	vertex_count = n_vertices;
	vertices = new int[vertex_count];
	normals = new int[vertex_count];
	texts = new int[vertex_count];
}

Indexed_Face::~Indexed_Face(){
	delete[] vertices;
	delete[] normals;
	delete[] texts;
}

/**
 * Conta o número de ocorrências de determinado símbolo em uma string.
 * @param  word   string da qual deseja-se contar os símbolos.
 * @param  symbol símbolo que será buscado na string.
 * @return        número de ocorrências do símbolo.
 */
int count_symbol(const std::string& word, char symbol){
	int pos = -1;
	int count = -1;

	do{
		pos = word.find(symbol, pos+1);
		count++;
	}
	while(pos != -1);

	return count;
}

/**
 * Lê um arquivo .obj, extrai as informações de vértices, normais e
 * texturas contidas nele e as escreve em arrays, separadamente.
 * @param obj_name       nome do arquivo .obj de onde deseja-se ler 
 *                       as informações.
 * @param vertices       ponteiro para um array de Vertex que contem
 *                       todos os vértices lidos do arquivo .obj.
 * @param normals        ponteiro para um array de Vertex que contem 
 *                       todos os vetores normais lidos do .obj.
 * @param uvs            ponteiro para um array de Vertex que contem
 *                       todas as coordenadas de textura lidas do .obj.
 * @param vertex_indices ponteiro para um array de int que contem 
 *                       todos os índices que referenciam os elementos 
 *                       no array de vértices, lidos do .obj.
 * @param normal_indices ponteiro para um array de int que contem 
 *                       todos os índices que referenciam os elementos 
 *                       no array de vetores normais, lidos do .obj.
 * @param uv_indices     ponteiro para um array de int que contem 
 *                       todos os índices que referenciam os elementos 
 *                       no array de coordenadas de textura, lidos 
 *                       do arquivo .obj.
 * @param vertex_count   referência para um inteiro que contem
 *                       o número de vértices lidos do arquivo.
 * @param normal_count   referência para um inteiro que contem
 *                       o número de normais lidas do arquivo.
 * @param uv_count       referência para um inteiro que contem
 *                       o número de coordenadas de textura lidas 
 *                       do arquivo.
 * @param face_count referência para um inteiro que contem
 *                       o número de triângulos formados pelos 
 *                       vértices lidos do arquivo.
 */
void load_data(	const std::string& obj_name,
				std::vector<glm::vec3> &vertices,
				std::vector<glm::vec3> &normals,
				std::vector<glm::vec2> &uvs, 
				std::vector<Indexed_Face> &faces,
				int& vertex_count, 
				int& normal_count, 
				int& uv_count,
				int& face_count){
	
	std::fstream input_obj(obj_name, std::ios_base::in);
	std::string line;

	vertex_count = 0;
	normal_count = 0; 
	uv_count = 0;
	face_count = 0;

	while(std::getline(input_obj, line)){
		if(line[0] == '#') continue;

		if(line.substr(0, 2) == "v "){
			vertex_count++;
		}
		else if(line.substr(0,3) == "vn "){
			normal_count++;
		}
		else if(line.substr(0,3) == "vt "){
			uv_count++;
		}
		else if(line.substr(0,2) == "f "){
			face_count++;
		}
	}	
	/*std::cout << "Vertices: " << vertex_count << std::endl;
	std::cout << "Normals: " << normal_count << std::endl;
	std::cout << "Textures: " << uv_count << std::endl;*/

	vertices.resize(vertex_count);
	if(normal_count > 0) normals.resize(normal_count);
	if(uv_count > 0) uvs.resize(uv_count);
	faces.resize(face_count);

	input_obj.clear();
	input_obj.seekg(0, std::ios::beg);

	int v_index = 0, vn_index = 0, uv_index = 0, face_index = 0;

	while(std::getline(input_obj, line)){
		if(line[0] == '#') continue;

		if(line.substr(0, 2) == "v "){
			std::stringstream(line.substr(2)) >> vertices[v_index].x
											  >> vertices[v_index].y
											  >> vertices[v_index].z;
			v_index++;
		}
		else if(line.substr(0, 3) == "vn "){
			std::stringstream(line.substr(3)) >> normals[vn_index].x
											  >> normals[vn_index].y
											  >> normals[vn_index].z;
			vn_index++;
		}
		else if(line.substr(0, 3) == "vt "){
			std::stringstream(line.substr(3)) >> uvs[uv_index].x
											  >> uvs[uv_index].y;
			uv_index++;
		}
		else if(line.substr(0,2) == "f "){
			std::string index_block;

			std::stringstream(line.substr(2)) >> index_block;

			int count = count_symbol(line);
			int first = index_block.find("/");
			int last = index_block.rfind("/");
			int int_ignore;
			char char_ignore;

			faces[face_index].vertex_count = count;
			std::stringstream line_stream = std::stringstream(line.substr(2));	
			
			if(first == -1){

				for(int i = 0; i < count; i++){
					line_stream >> faces[face_index].vertices[i];
				}
			}
			else if(first == last){

				for(int i = 0; i < count; i++){
					line_stream >> faces[face_index].vertices[i];
					line_stream >> char_ignore;
					line_stream >> faces[face_index].texts[i];
				}
			}
			else if(last - first == 1){

				for(int i = 0; i < count; i++){
					line_stream >> faces[face_index].vertices[i];
					line_stream >> char_ignore >> char_ignore;
					line_stream >> faces[face_index].normals[i];
				}
			}
			else{ // first != last

				for(int i = 0; i < count; i++){
					line_stream >> faces[face_index].vertices[i];
					line_stream >> char_ignore;
					line_stream >> faces[face_index].texts[i];
					line_stream >> char_ignore;
					line_stream >> faces[face_index].normals[i];
				}				
			}

			face_index++;
		}	
	}
}

/**
 * Agrupa os vértices, normais e coordenadas de textura em um único array de acordo
 * com a combinação apresentada pelos índices fornecidos.
 * @param vertices       array que contem uma lista de vértices.
 * @param normals        array que contem uma lista de vetores normais.
 * @param uvs            array que contem uma lista de coordenadas de textura.
 * @param vertex_indices array de índices inteiros que mapeaiam os
 *                       elementos que se encontram em vertices.
 * @param normal_indices array de índices inteiros que mapeaiam os
 *                       elementos que se encontram em normals.
 * @param uv_indices     array de índices inteiros que mapeaiam os
 *                       elementos que se encontram em uvs.
 * @param face_count número de triângulos formados pelos vértices.
 * @param data           ponteiro para um array de Vertex254s, que contêm 
 *                       todos os vértices, normais e coordenadas de 
 *                       textura lidas dos três primeiros parâmetros, 
 *                       agrupados de acordo com a combinação de índices 
 *                       passados nos três últimos parâmetros.
 */
void group_data(const std::vector<glm::vec3> &vertices,
				const std::vector<glm::vec3> &normals,
				const std::vector<glm::vec2> &uvs, 
				const std::vector<Indexed_Face> &faces,
				int& face_count,
				std::vector<Vertex> &data,
				const bool triangulate){

	Vertex aux;
	int n_vertices;
	face_count = 0;

	for(int i = 0; i < faces.size(); i++){

		n_vertices = faces[i].vertex_count;

		if(!triangulate || n_vertices == 3){
			for(int j = 0; j < n_vertices; j++){
				aux.vertices = vertices[faces[i].vertices[j]-1];

				if(!normals.empty()) 
					aux.normals = normals[faces[i].normals[j]-1];

				if(!uvs.empty())
					aux.texts = uvs[faces[i].texts[j]-1];

				data.push_back(aux);
			}
			face_count++;
		}

		else{
			for(int j = 0; j < n_vertices-1; j += 2){
				for(int k = j; k < j+3; k++){
					aux.vertices = vertices[faces[i].vertices[k % n_vertices]-1];

					if(!normals.empty()) 
						aux.normals = normals[faces[i].normals[k % n_vertices]-1];

					if(!uvs.empty())
						aux.texts = uvs[faces[i].texts[k % n_vertices]-1];

					data.push_back(aux);
				}
				face_count++;
			}
		}
	}
}

/**
 * Extrai a informação sobre vértices, normais e coordenadas de textura 
 * de um arquivo .obj e escreve os dados obtidos de forma agrupada, em 
 * um único array.
 * 
 * @param [in] obj_name			nome do arquivo .obj do qual deseja-se extrair 
 * 								os dados.
 * 								
 * @param [out] face_count 	quantidade de triângulos lidos no arquivo.
 * 
 * @param [out] data 			array de estruturas Vertex254, que contêm todos os
 * 								vértices, normais e coordenadas de texturas 
 * 								lidos do arquivo.
 */

/**
 * Extrai a informação sobre vértices, normais e coordenadas de textura 
 * de um arquivo .obj e escreve os dados obtidos de forma agrupada, em 
 * um único array.
 * @param obj_name       nome do arquivo .obj do qual deseja-se extrair 
 *                       os dados.
 * @param face_count quantidade de triângulos lidos no arquivo.
 * @param data           array de estruturas Vertex254, que contêm todos os
 *                       vértices, normais e coordenadas de texturas 
 *                       lidos do arquivo.
 */
void load_grouped_data(	const std::string& obj_name,
						int& face_count,
						std::vector<Vertex> &data,
						const bool triangulate){
	
	std::vector<glm::vec3> v, vn;
	std::vector<glm::vec2> vt;
	std::vector<Indexed_Face> face;
	int vertex_count, normal_count, vt_count;

	load_data(	obj_name,
				v, vn, vt,
				face,
				vertex_count,
				normal_count,
				vt_count,
				face_count);

	group_data(	v, vn, vt,
				face,
				face_count,
				data,
				triangulate);
}

/**
 * Lê um arquivo de texto e retorna seu conteúdo na forma
 * de uma string. Neste contexto, é usada para ler arquivos que contêm
 * código-fonte de shaders, no formato .glsl.
 * @param  file_name nome do arquivo a ser lido.
 * @return           string com o conteúdo do arquivo.
 */
std::string load_shader(const std::string& file_name){
	std::fstream file(file_name, std::ios_base::in);
	if(file.fail()) std::cout << "Arquivo " << file_name << " não encontrado" << std::endl;
	std::string code;
	std::string line;

	while(std::getline(file, line)){
		code.append(line);
		code.append("\n");
	}

	file.close();

	//std::cout << code << std::endl;

	return code;
}
