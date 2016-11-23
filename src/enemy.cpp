#include <scene.h>

/**
 * Contrutor vazio.
 */
Enemy::Enemy(){}

Enemy::~Enemy(){
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteSamplers(1, &samp);
	glDeleteTextures(1, &text_gl_name);
}

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
	
	glActiveTexture(GL_TEXTURE0);
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

void Enemy::draw(float x, float z, float speed, float *dist){
	
	glm::vec2 dir(this->x() - x,this->z() - z);
	*dist = glm::length(dir);
	if(*dist > 0.001f){
		dir = glm::normalize(dir);
		float angle = glm::orientedAngle(dir, glm::normalize(-glm::vec2(m_model[2].x, m_model[2].z)));

		localRotate(0, 1, 0, angle);
		localTranslate(0, 0, speed*0.15);
	}

	// static int oc = 0;

	// if(oc++ % 240){}
	// else{
	// 	std::cout << "PLAYER: " << x << ", " << z << std::endl;
	// 	std::cout << "ENEMY : " << this->x() << ", " << this->z() << std::endl;
	// }
	
	Object::draw();

}