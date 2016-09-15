#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <loader.h>

std::string SHADERS, DATA, TEXTURES;

void initDirectories(const char *location){
	std::string str_location(location);
	str_location = str_location.substr(0, str_location.rfind('/')+1);
	SHADERS = DATA = TEXTURES = str_location;
	SHADERS = SHADERS + "../shaders";
	DATA = DATA + "../data";
	TEXTURES = TEXTURES + "../textures";
}

////////////////////////
// Callback Functions //
////////////////////////
void error_callback(int error, const char* description)
{
	std::cerr << description;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(int argc, const char* argv[]){

	initDirectories(argv[0]);

	/////////////////////////
	// GLFW Initialization //
	/////////////////////////
	if (!glfwInit()){
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	///////////////////////////
	// Window Initialization //
	///////////////////////////
	GLFWwindow* window = glfwCreateWindow(600, 600, "Projeto de Computação Gráfica", NULL, NULL);
	
	if (!window){
		glfwTerminate();
		std::cerr << "Failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(window, 200, 10);
	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);

	/////////////////////////
	// GLEW Initialization //
	/////////////////////////
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	///////////////
	// Main Loop //
	///////////////
	while (!glfwWindowShouldClose(window))
	{

		const GLfloat background_color[] = {0.5f, 0.5f , 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/////////////////
	// Destroy All //
	/////////////////
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}