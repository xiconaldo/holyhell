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
Terrain *t;
Object *ob;
Object *entity;
Grass *grass;
float d = 0.1f;
glm::mat4 proj;
glm::vec4 light = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
glm::mat4 rotP = glm::rotate(0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotN = glm::rotate(-0.005f, glm::vec3(1.0f, 0.0f, 0.0f));

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
	Object::setBaseTextLocation(TEXTURES);
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

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->translate(0, 0, -0.01f);

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->translate(0, 0, 0.01f);

	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->translate(-0.01f, 0, 0);

	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		c->translate(0.01f, 0, 0);

	if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT))
		entity->translate(0.01f, 0, 0);

	if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT))
		entity->translate(-0.01f, 0, 0);

	if (key == GLFW_KEY_B && (action == GLFW_PRESS || action == GLFW_REPEAT))
		entity->translate(0.0f, 0, 0.01f);

	if (key == GLFW_KEY_Y && (action == GLFW_PRESS || action == GLFW_REPEAT))
		entity->translate(0.0f, 0, -0.01f);

	if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		light = rotP * light;
		glUniform3fv(3, 1, glm::value_ptr(light));
	}

	if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT)){
		light = rotN * light;
		glUniform3fv(3, 1, glm::value_ptr(light));
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key == GLFW_KEY_K && action == GLFW_PRESS){
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(10.0f);
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
	static double prev_x = 0.0, prev_y = 0.0;
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if(xpos-prev_x < 0)
		c->rotate(0, 1, 0, 0.02);

	if(xpos-prev_x > 0)
		c->rotate(0, 1, 0, -0.02);

	if(ypos-prev_y < 0)
		c->rotate(1, 0, 0, 0.02);

	if(ypos-prev_y > 0)
		c->rotate(1, 0, 0, -0.02);

	prev_x = xpos;
	prev_y = ypos;
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
	//GLFWwindow* window = glfwCreateWindow(800, 600, "Projeto de Computação Gráfica", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Projeto de Computação Gráfica", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(1366, 768, "Projeto de Computação Gráfica", glfwGetPrimaryMonitor(), NULL);
	
	if (!window){
		glfwTerminate();
		std::cerr << "Failed to create window" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(window, 200, 10);
	glfwMakeContextCurrent(window);
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

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

	std::string h_names[] = {"height_vertex.glsl", "default_frag.glsl"};
	GLuint height_program = create_program(2, flags, h_names);

	std::string g_names[] = {"grass_vertex.glsl", "grass_frag.glsl"};
	GLuint grass_program = create_program(2, flags, g_names);


	/////////////
	// Objects //
	/////////////
	
	t = new Terrain;
	t->loadData("plane.obj", "map.ktx");
	t->bindProgram(ter_program);

	ob = new Object;
	ob->loadData("plane.obj");
	ob->bindProgram(simple_program);

	grass = new Grass;
	grass->loadData("grass.obj");
	grass->bindProgram(grass_program);
	grass->scale(0.01f, 0.01f, 0.01f);

	entity = new Object;
	entity->loadData("tree.obj", "pine_tree.ktx");
	entity->bindProgram(height_program);
	entity->scale(0.01f, 0.01f, 0.01f);

	c = new Camera(0, 0, 2, 0, 0, -1);
	proj = glm::infinitePerspective(3.14f/4.0f, 16.0f/9.0f, 0.001f);	

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	
	///////////////
	// Main Loop //
	///////////////
	while (!glfwWindowShouldClose(window))
	{

		const GLfloat background_color[] = {0.5294f, 0.8078f , 0.9804f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);

		GLfloat min = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &min);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glUseProgram(ter_program);
		c->bindProgram(ter_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		t->draw();

		glCullFace(GL_BACK);
		glUseProgram(simple_program);
		c->bindProgram(simple_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		ob->draw();

		glDisable(GL_CULL_FACE);

		glUseProgram(height_program);
		c->bindProgram(height_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		entity->draw();

		glUseProgram(grass_program);
		c->bindProgram(grass_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		grass->draw();

		
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