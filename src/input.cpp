#include <input.h>

Input& Input::instance(){
	static Input instance;
	return instance;
}

Input::Input(){
	keyboard = new int[350];

	for(int i = 0; i < 350; i++)
		keyboard[i] = 0;

	mouse = new int[12];

	for(int i = 0; i < 12; i++)
		mouse[i] = 0;

	joystickButtons = new unsigned char[30];

	for(int i = 0; i < 30; i++)
		joystickButtons[i] = 0;
}

Input::~Input(){
	delete[] keyboard;
	delete[] mouse;
	delete[] joystickButtons;
}

// KEYBOARD
void Input::updateKey(int key, int action){
	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		if (keyboard[key] != GLFW_REPEAT)
			keyboard[key]++;
	}
	else{
		keyboard[key] = GLFW_RELEASE;
	}	
}

void Input::updateKeys(){

	for(int i = 0; i < 350; i++)
		if(keyboard[i] == GLFW_PRESS)
			keyboard[i] = GLFW_REPEAT;
}

bool Input::isJustPressedKey(int key){
	return keyboard[key] == GLFW_PRESS;
}

bool Input::isReleasedKey(int key){
	return keyboard[key] == GLFW_RELEASE;
}

bool Input::isPressedKey(int key){
	return keyboard[key] == GLFW_REPEAT;
}

int  Input::getStateKey(int key){
	return keyboard[key];	
}

// MOUSE
void Input::updateMouseMotion(double xpos, double ypos){
	static double prev_x = xpos, prev_y = ypos;

	mouse_dx = xpos-prev_x;
	mouse_dy = ypos-prev_y;

	prev_x = xpos;
	prev_y = ypos;
}

void Input::updateMouseButton(int button, int action){
	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		if (mouse[button] != GLFW_REPEAT)
			mouse[button]++;
	}
	else{
		mouse[button] = GLFW_RELEASE;
	}	
}

bool Input::isJustPressedButton(int button){
	return mouse[button] == GLFW_PRESS;
}

bool Input::isReleasedButton(int button){
	return mouse[button] == GLFW_RELEASE;
}

bool Input::isPressedButton(int button){
	return mouse[button] == GLFW_REPEAT;
}

int  Input::getStateButton(int button){
	return mouse[button];	
}

double Input::moveMouseX(){
	return mouse_dx;
}

double Input::moveMouseY(){
	return mouse_dy;
}

void Input::resetMouse(){
	mouse_dx = 0.0f;
	mouse_dy = 0.0f;
}

// JOYSTICK
void Input::updateJoyButton(){
	int count;
	const unsigned char* axes = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
	for(int i = 0; i < count; i++){
		joystickButtons[i] = axes[i];
	}
}

bool Input::isJustPressedJoyButton(int key){
	return joystickButtons[key] == GLFW_PRESS;
}

bool Input::isReleasedJoyButton(int key){
	return joystickButtons[key] == GLFW_RELEASE;
}

bool Input::isPressedJoyButton(int key){
	return joystickButtons[key] == GLFW_REPEAT || joystickButtons[key] == GLFW_PRESS;
}

int  Input::getStateJoyButton(int key){
	return joystickButtons[key];	
}

void Input::updateJoyAxes(){

	int count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

	axis1_dx = axes[0];
	axis1_dy = axes[1];
	axis2_dx = axes[2];
	axis2_dy = axes[3];
}

double Input::moveJoyAxis1X(){
	return axis1_dx;
}

double Input::moveJoyAxis1Y(){
	return axis1_dy;
}

double Input::moveJoyAxis2X(){
	return axis2_dx;
}

double Input::moveJoyAxis2Y(){
	return axis2_dy;
}

void Input::resetJoyAxes(){
	axis1_dx = 0.0f;
	axis1_dy = 0.0f;
	axis2_dx = 0.0f;
	axis2_dy = 0.0f;
}