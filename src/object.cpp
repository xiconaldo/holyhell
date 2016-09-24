#include <scene.h>

/**
 * Contrutor vazio.
 */
Object::Object(){}

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Object::loadData(const std::string& object_name){
	std::vector<Vertex> data;
	load_grouped_data(base_data_location + object_name, triangle_count, data);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);
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
void Object::rotate(int xc, int yc, int zc, float degrees){
	
	if(xc) rotation.x += degrees;
	if(yc) rotation.y += degrees;
	if(zc) rotation.z += degrees;

	updateModel();
	updateModelMatrix();
}

/**
 * Adiciona uma translação à matriz model. A translação sempre é a 
 * última operação a ser aplicada.
 * @param x valor da translação no eixo x.
 * @param y valor da translação no eixo y.
 * @param z valor da translação no eixo z.
 */
void Object::translate(float x, float y, float z){
	translation += glm::vec3(x, y, z);
	updateModel();
	updateModelMatrix();
}

/**
 * Adiciona uma escala à matriz model. A escala sempre é a 
 * primeira operação a ser aplicada.
 * @param x fator de escala no eixo x.
 * @param y fator de escala no eixo y.
 * @param z fator de escala no eixo z.
 */
void Object::scale(float x, float y, float z){
	scalation *= glm::vec3(x, y, z);
	updateModel();
	updateModelMatrix();
}

/**
 * Associa os valores do vertex buffer e a matriz model aos 
 * respectivos atributos do programa especificado.
 * @param program programa ao qual os objetos serão associados.
 */
void Object::makeActiveOnProgram(GLuint program){

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	vertex_location = glGetAttribLocation(program, "vertex");
	normal_location = glGetAttribLocation(program, "normal");
	model_location = glGetUniformLocation(program, "model");

	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));
	glEnableVertexAttribArray(vertex_location);

	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(normal_location);

	updateModelMatrix();
}

/**
 * Associa os valores do vertex buffer e a matriz model aos 
 * atributos e uniformes nas localizações especificadas.
 * @param vertexLocation localização do atributo referente aos
 *                       vértices.
 * @param normalLocation localização do atributo referente às
 *                       normais.
 * @param modelLocation  localização da variável uniforme referente 
 *                       à matriz model.
 */
void Object::makeActiveOnLocation(GLuint vertexLocation, GLuint normalLocation, GLuint modelLocation){

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	vertex_location = vertexLocation;
	normal_location = normalLocation;
	model_location = modelLocation;

	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertices));
	glEnableVertexAttribArray(vertex_location);

	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	glEnableVertexAttribArray(normal_location);

	updateModelMatrix();
}

/**
 * Atualiza a cópia local da matriz model.
 */
void Object::updateModel(){
	m_model = glm::translate(translation) *
			  glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
			  glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			  glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
			  glm::scale(scalation);
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
	translation = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scalation = glm::vec3(1.0f);
	m_model = glm::mat4(1.0f);
}

/**
 * Desenha o objeto na tela.
 */
void Object::draw(){
	glDrawArrays(GL_TRIANGLES, 0, 3*triangle_count);
}

/**
 * Seleciona o caminho do diretório onde se encontram as 
 * informações do objeto a serem carregadas.
 * @param location caminho do diretório.
 */
void Object::setBaseDataLocation(const std::string& location){
	base_data_location = location;
}

std::string Object::base_data_location = "";