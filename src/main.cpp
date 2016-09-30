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
Camera *c;
float d = 0.2f;
glm::mat4 proj;

void initDirectories(const char *location){
	std::string SHADERS;
	std::string DATA;
	std::string TEXTURES;
	std::string str_location(location);
	str_location = str_location.substr(0, str_location.rfind('/')+1);
	SHADERS = str_location + "../shaders/";
	DATA = str_location + "../data/";
	TEXTURES = str_location + "../textures/";
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

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->rotate(0, 0, 1, 0.01);

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->rotate(0, 0, 1, -0.01);

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->rotate(1, 0, 0, 0.01);

	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->rotate(1, 0, 0, -0.01);

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		d += 0.001;
		c->translate(0, 0, -0.01f);
		proj = glm::infinitePerspective(3.14f/4, 16.0f/9.0f, 0.1f);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
	}

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		d -= 0.001;
		c->translate(0, 0, 0.01f);
		proj = glm::infinitePerspective(3.14f/4, 16.0f/9.0f, 0.1f);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
	}
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
	GLFWwindow* window = glfwCreateWindow(1366, 768, "Projeto de Computação Gráfica", glfwGetPrimaryMonitor(), NULL);
	
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

	GLenum flags[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
	std::string names[] = {"default_vertex.glsl", "default_frag.glsl"};
	GLuint simple_program = create_program(2, flags, names);
	//glUseProgram(simple_program);
	

	/////////////
	// Objects //
	/////////////

	/*GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	Object ico;
	ico.loadData("ico.obj");
	ico.makeActiveOnProgram(simple_program);*/

	GLuint vao2;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	
	Terrain t;
	t.loadData("plane.obj", "map.ktx");
	t.makeActiveOnProgram(ter_program);
	t.scale(0.8f, 0.8f, 1.0f);

	c = new Camera();
	c->makeActiveOnProgram(ter_program);

	proj = glm::infinitePerspective(3.14f/4, 16.0f/9.0f, 0.1f);
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_DEPTH_CLAMP);

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
		//ico.rotate(1,1,1,0.001);
		//c.rotate(1,0,0, 0.001);
		//ico.scale(1.1, 1.1, 1.1);
		/*glBindVertexArray(vao);
		glUseProgram(simple_program);
		ico.draw();*/
		glBindVertexArray(vao2);
		glUseProgram(ter_program);
		//t.rotate(1,1,1, 0.005);
		t.draw();
		

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