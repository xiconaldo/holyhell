#include <scene.h>

/**
 * Carrega os dados do objeto em um buffer.
 * @param object_name nome do arquivo onde se encontram os dados.
 */
void Player::loadData(const std::string& object_name, const std::string& text_name){

	load_grouped_data(base_data_location + object_name, triangle_count, data, true);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), (void*)data.data(), GL_STATIC_DRAW);

	GLuint player_position;
	float position[2];
	position[0] = m_model[3].x;
	position[1] = m_model[3].z;

	glGenBuffers(1, &player_position);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, player_position);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float), (void*)position, GL_STATIC_DRAW);

	glGenSamplers(1, &samp);
	glSamplerParameteri(samp, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samp, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glBindSampler(text_target, samp);
	
	glActiveTexture(GL_TEXTURE0);
	KTX_error_code_t error = ktxLoadTextureN((base_text_location + text_name).c_str(), &text_gl_name, &text_target, NULL, NULL, NULL, NULL, NULL);
	verifyTextError(error);
}

/**
 * Contrutor vazio.
 */
Player::Player(){}

void Player::draw(){
	int factor = 1.0f;

	if(Input::instance().getStateKey(GLFW_KEY_LEFT_SHIFT))
		factor = 2.0f;
	
	if(Input::instance().getStateKey(GLFW_KEY_W))
		localTranslate(0.1f * factor, 0.0f, 0.0f);
	if(Input::instance().getStateKey(GLFW_KEY_S))
	 	localTranslate(-0.1f * factor, 0.0f, 0.0f);
	 if(Input::instance().getStateKey(GLFW_KEY_A))
		localTranslate(0.0f, 0.0f, -0.1f * factor);
	if(Input::instance().getStateKey(GLFW_KEY_D))
	 	localTranslate(0.0f, 0.0f, 0.1f * factor);

	if(Input::instance().moveMouseX())
		localRotate(0, 1, 0, -0.01f * Input::instance().moveMouseX());


	if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
		if(Input::instance().isJustPressedJoyButton(JOY_R1))
			factor = 2.0f;

		if (Input::instance().moveJoyAxis1X() > 0.0f)
			localTranslate(0.0f, 0.0f, 0.1f * factor);
		else if(Input::instance().moveJoyAxis1X() < 0.0f)
			localTranslate(0.0f, 0.0f, -0.1f * factor);

		if (Input::instance().moveJoyAxis1Y() > 0.0f)
			localTranslate(-0.1f * factor, 0.0f, 0.0f);
		else if (Input::instance().moveJoyAxis1Y() < 0.0f)
			localTranslate(0.1f * factor, 0.0f, 0.0f);

		if(Input::instance().moveJoyAxis2X())
			localRotate(0, 1, 0, -0.01f * Input::instance().moveJoyAxis2X()*3);
	}

	const float position[] = {m_model[3].x, m_model[3].z};

	void *content;
	content = glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_WRITE);
	memcpy(content, (const void*)position, 2*sizeof(float));

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	Object::draw();

}