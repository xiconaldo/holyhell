#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <loader.h>
#include <shader.h>
#include <scene.h>

#define KTX_OPENGL 1
#include <ktx.h>

CreateProgram create_program;
std::string TEXTURES;

void initDirectories(const char *location){
	std::string SHADERS;
	std::string DATA;
	std::string str_location(location);
	str_location = str_location.substr(0, str_location.rfind('/')+1);
	SHADERS = DATA = TEXTURES = str_location;
	SHADERS = SHADERS + "../shaders/";
	DATA = DATA + "../data/";
	TEXTURES = TEXTURES + "../textures/";
	create_program.baseLocation = SHADERS;
	Object::setBaseDataLocation(DATA);
	Terrain::setBaseTextLocation(TEXTURES);
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

	//////////////////////
	// Program begining //
	//////////////////////
	GLenum ter_flags[] = {GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER};
	std::string ter_names[] = {"terrain_vertex.glsl", "terrain_tess_ct.glsl", "terrain_tess_ev.glsl", "terrain_frag.glsl"};
	GLuint ter_program = create_program(4, ter_flags, ter_names);
	glUseProgram(ter_program);

	GLenum flags[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
	std::string names[] = {"default_vertex.glsl", "default_frag.glsl"};
	GLuint simple_program = create_program(2, flags, names);
	glUseProgram(simple_program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	Object ico;
	ico.loadData("ico.obj");
	ico.makeActiveOnProgram(simple_program);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	///////////////
	// Main Loop //
	///////////////
	while (!glfwWindowShouldClose(window))
	{

		const GLfloat background_color[] = {1.0f, 1.0f , 1.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);

	//	GLfloat min = 0.0f;
	//	glClearBufferfv(GL_DEPTH, 0, &min);
		
		//glDrawArrays(GL_PATCHES, 0, 6);
		//iso.translate(0,0.001,0.0001);
		ico.rotate(1,1,1,0.001);
		//c.rotate(1,0,0, 0.001);
		//ico.scale(1.1, 1.1, 1.1);
		ico.draw();
		

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