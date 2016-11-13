#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <loader.h>
#include <shader.h>
#include <scene.h>
#include <input.h>

#define KTX_OPENGL 1
#include <ktx.h>

CreateProgram create_program;
Camera *c;
Terrain *t;
Object *plane;
Object *tree, *tree2, *tree3, *stark;
Player *me;
Grass *grass;
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

	Input::instance().updateKey(key, action);
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::instance().updateMouseMotion(xpos, ypos);
}

void joystick_callback(int joy, int event)
{
    if (event == GLFW_CONNECTED){
        std::cout << "The joystick was connected" << std::endl;
    }
    else if (event == GLFW_DISCONNECTED){
        std::cout << "The joystick was disconnected" << std::endl;
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
	//GLFWwindow* window = glfwCreateWindow(800, 600, "Projeto de Computação Gráfica", glfwGetPrimaryMonitor(), NULL);
	//GLFWwindow* window = glfwCreateWindow(1024, 768, "Projeto de Computação Gráfica", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(1366, 768, "Projeto de Computação Gráfica", glfwGetPrimaryMonitor(), NULL);
	
	GLFWwindow* window = glfwCreateWindow(1024, 576, "Projeto de Computação Gráfica", NULL, NULL);
	
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
	glfwSetJoystickCallback(joystick_callback);
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
	GLenum flags[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
	GLenum ter_flags[] = {GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_FRAGMENT_SHADER};

	std::string ter_names[] = {"terrain_vertex.glsl", "terrain_tess_ct.glsl", "terrain_tess_ev.glsl", "terrain_frag.glsl"};
	GLuint ter_program = create_program(4, ter_flags, ter_names);
	
	std::string h_names[] = {"height_vertex.glsl", "default_frag.glsl"};
	GLuint height_program = create_program(2, flags, h_names);

	std::string g_names[] = {"grass_vertex.glsl", "default_frag.glsl"};
	GLuint grass_program = create_program(2, flags, g_names);


	/////////////
	// Objects //
	/////////////
	
	t = new Terrain;
	t->loadData("plane.obj", "dirt.ktx", "map.ktx");
	t->bindProgram(ter_program);

	grass = new Grass;
	grass->loadData("grass.obj", "green.ktx");
	grass->bindProgram(grass_program);
	grass->scale(0.01f, 0.01f, 0.01f);

	me = new Player;
	me->loadData("iron_man.obj", "iron_man.ktx");
	me->bindProgram(height_program);
	me->scale(0.01f, 0.01f, 0.01f);
	me->translate(0.1f, 0.0f, 0.0f);

	tree = new Object;
	tree->loadData("tree.obj", "pine_tree.ktx");
	tree->bindProgram(height_program);
	tree->scale(0.015f);
	tree->scale(3.0f);

	tree2 = new Object;
	tree2->loadData("tree.obj", "pine_tree.ktx");
	tree2->bindProgram(height_program);
	tree2->scale(0.01f, 0.01f, 0.01f);
	tree2->scale(3.0f);
	tree2->translate(0.1f, 0.0f, 0.1f);

	tree3 = new Object;
	tree3->loadData("tree.obj", "pine_tree.ktx");
	tree3->bindProgram(height_program);
	tree3->scale(0.01f, 0.01f, 0.01f);
	tree3->scale(3.0f);
	tree3->translate(-0.1f, 0.0f, 0.1f);

	//c = new Camera(0, 0.2f, -0.1f, me->x(), me->y(), 0.2f);
	c = new Camera(0, 0, 0, me->x(), 0.0f, me->z());
	proj = glm::infinitePerspective(3.14f/4.0f, 16.0f/9.0f, 0.001f);	

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	
	///////////////
	// Main Loop //
	///////////////
	
	double lastTime, currentTime;
	int frame = 0;

	while (!glfwWindowShouldClose(window))
	{

		if (Input::instance().getStateKey(GLFW_KEY_A))
			c->rotate(0, 0, 1, 0.01);

		if (Input::instance().getStateKey(GLFW_KEY_D))
			c->rotate(0, 0, 1, -0.01);

		// if (Input::instance().getStateKey(GLFW_KEY_W))
		// 	c->translate(0, 0, -0.01f);

		// if (Input::instance().getStateKey(GLFW_KEY_S))
		// 	c->translate(0, 0, 0.01f);

		static float vertical_angle = 0.0f;

		if(Input::instance().moveMouseX()){
			if(vertical_angle){
				c->rotate(1, 0, 0, -vertical_angle);
				c->rotate(0, 1, 0, -0.01f * Input::instance().moveMouseX());
				c->rotate(1, 0, 0, vertical_angle);
			}
			else{
				c->rotate(0, 1, 0, -0.01f * Input::instance().moveMouseX());
			}
		}
		if(Input::instance().moveMouseY()){
			c->rotate(1, 0, 0, -0.01f * Input::instance().moveMouseY());
			vertical_angle += -0.01f * Input::instance().moveMouseY();
		}

		if(glfwJoystickPresent(GLFW_JOYSTICK_1)){

			Input::instance().updateJoyButton();
			Input::instance().updateJoyAxes();

			c->rotate(0, 1, 0, -0.01f * Input::instance().moveJoyAxis2X()*7);
			c->rotate(1, 0, 0, -0.01f * Input::instance().moveJoyAxis2Y()*5);

			if (Input::instance().moveJoyAxis1X() < 0.0f)
				c->translate(-0.01f, 0, 0);

			if (Input::instance().moveJoyAxis1X() > 0.0f)
				c->translate(0.01f, 0, 0);

			if (Input::instance().moveJoyAxis1Y() < 0.0f)
				c->translate(0, 0, -0.01f);

			if (Input::instance().moveJoyAxis1Y() > 0.0f)
				c->translate(0, 0, 0.01f);

			if(Input::instance().isJustPressedJoyButton(1))
				glfwSetWindowShouldClose(window, GL_TRUE);
		}


		lastTime = glfwGetTime();

		const GLfloat background_color[] = {0.5294f, 0.8078f , 0.9804f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);

		GLfloat far = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &far);

		glUseProgram(ter_program);
		c->bindProgram(ter_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		t->draw();

		glUseProgram(grass_program);
		c->bindProgram(grass_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		grass->draw();

		glUseProgram(height_program);
		c->bindProgram(height_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		tree->draw();
		tree2->draw();
		tree3->draw();
		me->draw();

		Input::instance().resetMouse();
		Input::instance().resetJoyAxes();
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		currentTime = glfwGetTime();

		frame++;
		if(frame%120 == 0){
			std::cout << int(1/(currentTime - lastTime)) << " fps" << std::endl;
		}
		
	}

	/////////////////
	// Destroy All //
	/////////////////
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}