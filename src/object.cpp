#include <scene.h>

/**
 * Contrutor vazio.
 */
Object::Object(){}

Object::~Object(){
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteSamplers(1, &samp);
	glDeleteTextures(1, &text_gl_name);
}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Object::loadData(const std::string& object_name, const std::string& text_name){

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

/**
 * Adiciona uma rotação à matriz model. A rotação sempre é a segunda 
 * operação aplicada e é aplica na ordem z, y, x.
 * @param xc      indica se o modelo deve ou não ser rotacionado 
 *                no eixo x.
 * @param yc      indica se o modelo deve ou não ser rotacionado 
 *                no eixo y.
 * @param zc      indica se o modelo deve ou não ser rotacionado 
 *                no eixo z.
 * @param degrees ângulo em radianos que o objeto deverá rotacionar.
 */
void Object::localRotate(int xc, int yc, int zc, float degrees){
	m_model = m_model * glm::rotate(degrees, glm::vec3(xc, yc, zc));
	updateModelMatrix();
}

/**
 * Adiciona uma translação à matriz model. A translação sempre é a 
 * última operação a ser aplicada.
 * @param x valor da translação no eixo x.
 * @param y valor da translação no eixo y.
 * @param z valor da translação no eixo z.
 */
void Object::localTranslate(float x, float y, float z){
	m_model = m_model * glm::translate(glm::vec3(x, y, z));
	updateModelMatrix();
}

/**
 * Adiciona uma escala à matriz model. A escala sempre é a 
 * primeira operação a ser aplicada.
 * @param x fator de escala no eixo x.
 * @param y fator de escala no eixo y.
 * @param z fator de escala no eixo z.
 */
void Object::localScale(float x, float y, float z){
	m_model = m_model * glm::scale(glm::vec3(x, y, z));
	updateModelMatrix();
}

void Object::localScale(float xyz){
	m_model = m_model * glm::scale(glm::vec3(xyz));
	updateModelMatrix();
}

void Object::rotate(int xc, int yc, int zc, float degrees){
	m_model = glm::rotate(degrees, glm::vec3(xc, yc, zc)) * m_model;
	updateModelMatrix();
}

void Object::translate(float x, float y, float z){
	m_model = glm::translate(glm::vec3(x, y, z)) * m_model;
	updateModelMatrix();
}

void Object::scale(float x, float y, float z){
	m_model = glm::scale(glm::vec3(x, y, z)) * m_model;
	updateModelMatrix();
}

void Object::scale(float xyz){
	m_model = glm::scale(glm::vec3(xyz)) * m_model;
	updateModelMatrix();
}

float Object::x(){
	return m_model[3].x;
}

float Object::y(){
	return m_model[3].y;
}

float Object::z(){
	return m_model[3].z;
}

/**
 * Associa os valores do vertex buffer e a matriz model aos 
 * respectivos atributos do programa especificado.
 * @param program programa ao qual os objetos serão associados.
 */
void Object::bindProgram(GLuint program){

	vertex_location = glGetAttribLocation(program, "vertex");
	normal_location = glGetAttribLocation(program, "normal");
	uv_location 	= glGetAttribLocation(program, "uv");
	model_location = glGetUniformLocation(program, "model");

	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));
	glEnableVertexAttribArray(vertex_location);

	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(normal_location);

	glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texts));
	glEnableVertexAttribArray(uv_location);

	updateModelMatrix();
}

/**
 * Associa os dados da cópia local da matriz model com a 
 * variável uniforme especificada previamente.
 */
void Object::updateModelMatrix(){
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
}

/**
 * Recarrega a matriz identidade na model e zera as 
 * transformações acumuladas nas variáveis de escala, 
 * rotação e translação.
 */
void Object::resetMatrix(){
	m_model = glm::mat4(1.0f);
}

/**
 * Desenha o objeto na tela.
 */
void Object::draw(){
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(text_target, text_gl_name);

	glBindVertexArray(vao);
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));

	glDrawArrays(GL_TRIANGLES, 0, 3*triangle_count);
}

void Object::verifyTextError(KTX_error_code_t code){
	switch(code){
		case KTX_SUCCESS:
			//std::cout << "KTX_SUCCESS" << std::endl;
			break;
		case KTX_FILE_OPEN_FAILED:
			std::cout << "KTX_FILE_OPEN_FAILED" << std::endl;
			break;
		case KTX_FILE_WRITE_ERROR:
			std::cout << "KTX_FILE_WRITE_ERROR" << std::endl;
			break;
		case KTX_INVALID_OPERATION:
			std::cout << "KTX_INVALID_OPERATION" << std::endl;
			break;
		case  KTX_INVALID_VALUE:
			std::cout << "KTX_INVALID_VALUE" << std::endl;
			break;
		case  KTX_OUT_OF_MEMORY:
			std::cout << " KTX_OUT_OF_MEMORY," << std::endl;
			break;
		case KTX_UNEXPECTED_END_OF_FILE:
			std::cout << "KTX_UNEXPECTED_END_OF_FILE" << std::endl;
			break;
		case KTX_UNKNOWN_FILE_FORMAT:
			std::cout << "KTX_UNKNOWN_FILE_FORMAT" << std::endl;
			break;
		case KTX_UNSUPPORTED_TEXTURE_TYPE:
			std::cout << "KTX_UNSUPPORTED_TEXTURE_TYPE" << std::endl;
			break;
		case KTX_GL_ERROR:
			std::cout << "KTX_GL_ERROR" << std::endl;
			break;
		case  KTX_NOT_FOUND:
			std::cout << " KTX_NOT_FOUND" << std::endl;
			break;
	}
}

/**
 * Seleciona o caminho do diretório onde se encontram as 
 * informações do objeto a serem carregadas.
 * @param location caminho do diretório.
 */
void Object::setBaseDataLocation(const std::string& location){
	base_data_location = location;
}

void Object::setBaseTextLocation(const std::string& location){
	base_text_location = location;
}

std::string Object::base_data_location = "";
std::string Object::base_text_location = "";