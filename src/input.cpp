#include <input.h>

Input& Input::instance(){
	static Input instance;
	return instance;
}

Input::Input(){
	keyboard = new int[350];

	for(int i = 0; i < 350; i++)
		keyboard[i] = 0;
}

Input::~Input(){
	delete[] keyboard;
}

void Input::update(int key, int action){
	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		if (keyboard[key] != GLFW_REPEAT)
			keyboard[key]++;
	}
	else{
		keyboard[key] = GLFW_RELEASE;
	}	
}

bool Input::isJustPressed(int key){
	return keyboard[key] == GLFW_PRESS;
}

bool Input::isReleased(int key){
	return keyboard[key] == GLFW_RELEASE;
}

bool Input::isPressed(int key){
	return keyboard[key] == GLFW_REPEAT;
}

int  Input::getState(int key){
	return keyboard[key];	
}

// Mouse

void Input::update(double xpos, double ypos){
	static double prev_x = xpos, prev_y = ypos;

	dx = xpos-prev_x;
	dy = ypos-prev_y;

	prev_x = xpos;
	prev_y = ypos;
}

double Input::moveX(){
	return dx;
}

double Input::moveY(){
	return dy;
}

void Input::reset(){
	dx = 0.0f;
	dy = 0.0f;
}