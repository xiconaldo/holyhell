#include <loader.h>

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
 * @param triangle_count referência para um inteiro que contem
 *                       o número de triângulos formados pelos 
 *                       vértices lidos do arquivo.
 */
void load_data(	const std::string& obj_name,
				Vertex** const vertices,
				Vertex** const normals,
				Vertex** const uvs, 
				int** vertex_indices, 
				int** normal_indices, 
				int** uv_indices,
				int& vertex_count, 
				int& normal_count, 
				int& uv_count,
				int& triangle_count){
	
	std::fstream input_obj(obj_name, std::ios_base::in);
	std::string line;

	vertex_count = 0;
	normal_count = 0; 
	uv_count = 0;
	triangle_count = 0;

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
			triangle_count += count_symbol(line)-2;
		}
	}	
	std::cout << "Vertices: " << vertex_count << std::endl;
	std::cout << "Normals: " << normal_count << std::endl;
	std::cout << "Textures: " << uv_count << std::endl;

	*vertices = new Vertex[vertex_count];
	if(normal_count > 0) *normals = new Vertex[normal_count];
	else *normals = NULL;
	if(uv_count > 0) *uvs = new Vertex[uv_count];
	else *uvs = NULL;

	*vertex_indices = new int[3*triangle_count];
	if(normal_count > 0) *normal_indices = new int[3*triangle_count];
	else *normal_indices = NULL;
	if(uv_count > 0) *uv_indices = new int[3*triangle_count];
	else *uv_indices = NULL;

	input_obj.clear();
	input_obj.seekg(0, std::ios::beg);

	int v_index = 0, vn_index = 0, uv_index = 0, index_index = 0;

	while(std::getline(input_obj, line)){
		if(line[0] == '#') continue;

		if(line.substr(0, 2) == "v "){
			(*vertices)[v_index].w = 1.0f;
			std::stringstream(line.substr(2)) >> (*vertices)[v_index].x
											  >> (*vertices)[v_index].y
											  >> (*vertices)[v_index].z
											  >> (*vertices)[v_index].w;
			v_index++;
		}
		else if(line.substr(0, 3) == "vn "){
			(*normals)[vn_index].w = 1.0f;
			std::stringstream(line.substr(3)) >> (*normals)[vn_index].x
											  >> (*normals)[vn_index].y
											  >> (*normals)[vn_index].z
											  >> (*normals)[vn_index].w;
			vn_index++;
		}
		else if(line.substr(0, 3) == "vt "){
			(*uvs)[uv_index].w = 1.0f;
			std::stringstream(line.substr(3)) >> (*uvs)[uv_index].x
											  >> (*uvs)[uv_index].y
											  >> (*uvs)[uv_index].z
											  >> (*uvs)[uv_index].w;
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
			
			if(first == -1){
				std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index]
												  >> (*vertex_indices)[index_index+1]
												  >> (*vertex_indices)[index_index+2]
												  >> int_ignore;
				index_index += 3;

				if(count == 4){
					std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index+1]
													  >> int_ignore
													  >> (*vertex_indices)[index_index+2]
													  >> (*vertex_indices)[index_index];
					index_index += 3;
				}
			}
			else if(first == last){
				std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index]
													  >> char_ignore
													  >> (*normal_indices)[index_index]
													  >> (*vertex_indices)[index_index+1]
													  >> char_ignore
													  >> (*normal_indices)[index_index+1]
													  >> (*vertex_indices)[index_index+2]
													  >> char_ignore
													  >> (*normal_indices)[index_index+2];
				index_index += 3;

				if(count == 4){
					std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index+1]
													  >> char_ignore
													  >> (*normal_indices)[index_index+1]
													  >> int_ignore
													  >> char_ignore
													  >> int_ignore
													  >> (*vertex_indices)[index_index+2]
													  >> char_ignore
													  >> (*normal_indices)[index_index+2]
													  >> (*vertex_indices)[index_index]
													  >> char_ignore
													  >> (*normal_indices)[index_index];

					index_index += 3;
				}
			}
			else if(last - first == 1){
				std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index]
												  >> char_ignore >> char_ignore
												  >> (*normal_indices)[index_index]
												  >> (*vertex_indices)[index_index+1]
												  >> char_ignore >> char_ignore
												  >> (*normal_indices)[index_index+1]
												  >> (*vertex_indices)[index_index+2]
												  >> char_ignore >> char_ignore
												  >> (*normal_indices)[index_index+2];
				index_index += 3;

				if(count == 4){
					std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index+1]
													  >> char_ignore >> char_ignore
													  >> (*normal_indices)[index_index+1]
													  >> int_ignore
													  >> char_ignore >> char_ignore
													  >> int_ignore
													  >> (*vertex_indices)[index_index+2]
													  >> char_ignore >> char_ignore
													  >> (*normal_indices)[index_index+2]
													  >> (*vertex_indices)[index_index]
													  >> char_ignore >> char_ignore
													  >> (*normal_indices)[index_index];

					index_index += 3;
				}
			}
			else{ // first != last
				std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index]
												  >> char_ignore
												  >> (*normal_indices)[index_index]
												  >> char_ignore
												  >> (*uv_indices)[index_index]
												  >> (*vertex_indices)[index_index+1]
												  >> char_ignore
												  >> (*normal_indices)[index_index+1]
												  >> char_ignore
												  >> (*uv_indices)[index_index+1]
												  >> (*vertex_indices)[index_index+2]
												  >> char_ignore
												  >> (*normal_indices)[index_index+2]
												  >> char_ignore
												  >> (*uv_indices)[index_index+2];
				index_index += 3;

				if(count == 4){
					std::stringstream(line.substr(2)) >> (*vertex_indices)[index_index+1]
													  >> char_ignore
													  >> (*normal_indices)[index_index+1]
													  >> char_ignore
													  >> (*uv_indices)[index_index+1]
													  >> int_ignore
													  >> char_ignore
													  >> int_ignore
													  >> char_ignore
													  >> int_ignore
													  >> (*vertex_indices)[index_index+2]
													  >> char_ignore
													  >> (*normal_indices)[index_index+2]
													  >> char_ignore
													  >> (*uv_indices)[index_index+2]
													  >> (*vertex_indices)[index_index]
													  >> char_ignore
													  >> (*normal_indices)[index_index]
													  >> char_ignore
													  >> (*uv_indices)[index_index];

					index_index += 3;
				}
			}
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
 * @param triangle_count número de triângulos formados pelos vértices.
 * @param data           ponteiro para um array de Groups, que contêm 
 *                       todos os vértices, normais e coordenadas de 
 *                       textura lidas dos três primeiros parâmetros, 
 *                       agrupados de acordo com a combinação de índices 
 *                       passados nos três últimos parâmetros.
 */

void group_data(Vertex* const vertices,
				Vertex* const normals,
				Vertex* const uvs, 
				int* vertex_indices, 
				int* normal_indices, 
				int* uv_indices,
				int triangle_count,
				Group** const data){

	*data = new Group[triangle_count*3];

	for(int i = 0; i < triangle_count*3; i++){
		(*data)[i].vertices = vertices[vertex_indices[i]-1];
		if(normals != NULL) (*data)[i].normals = normals[normal_indices[i]-1];
		if(uvs != NULL) (*data)[i].texts = uvs[uv_indices[i]-1];
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
 * @param [out] triangle_count 	quantidade de triângulos lidos no arquivo.
 * 
 * @param [out] data 			array de estruturas Group, que contêm todos os
 * 								vértices, normais e coordenadas de texturas 
 * 								lidos do arquivo.
 */

/**
 * Extrai a informação sobre vértices, normais e coordenadas de textura 
 * de um arquivo .obj e escreve os dados obtidos de forma agrupada, em 
 * um único array.
 * @param obj_name       nome do arquivo .obj do qual deseja-se extrair 
 *                       os dados.
 * @param triangle_count quantidade de triângulos lidos no arquivo.
 * @param data           array de estruturas Group, que contêm todos os
 *                       vértices, normais e coordenadas de texturas 
 *                       lidos do arquivo.
 */
void load_grouped_data(	const std::string& obj_name,
						int& triangle_count,
						Group** const data){
	
	Vertex *v, *vn, *vt;
	int *v_index, *vn_index, *vt_index;
	int vertex_count, normal_count, vt_count;

	load_data(	obj_name,
				&v, &vn, &vt,
				&v_index,
				&vn_index,
				&vt_index,
				vertex_count,
				normal_count,
				vt_count,
				triangle_count);

	group_data(	v, vn, vt,
				v_index,
				vn_index,
				vt_index,
				triangle_count,
				data);

	if(v) delete[] v;
	if(vn) delete[] vn;
	if(vt) delete[] vt;
	if(v_index) delete[] v_index;
	if(vn_index) delete[] vn_index;
	if(vt_index) delete[] vt_index;
}

/**
 * Lê um arquivo de texto e escreve e retorna seu conteúdo na forma
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

	return code;
}
