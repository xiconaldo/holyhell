#include <scene.h>

/**
 * Contrutor vazio.
 */
Player::Player(){}

void Player::draw(){
	int factor = 1.0f;

	if(Input::instance().getStateKey(GLFW_KEY_LEFT_SHIFT))
		factor = 4.0f;

	if(Input::instance().getStateKey(GLFW_KEY_UP))
		localTranslate(0.0f, 0.0f, 0.05f * factor);
	if(Input::instance().getStateKey(GLFW_KEY_DOWN))
		localTranslate(0.0f, 0.0f, -0.05f * factor);
	if(Input::instance().getStateKey(GLFW_KEY_LEFT))
		localRotate(0, 1, 0, 0.05f);
	if(Input::instance().getStateKey(GLFW_KEY_RIGHT))
		localRotate(0, 1, 0, -0.05f);

	Object::draw();

}

void Player::adjustCamera(GLuint program){
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(m_model));
}