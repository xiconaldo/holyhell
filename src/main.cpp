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
Object **trees;
Player *me;
Grass *grass;
Enemy *slender;
Object **tomb;

glm::vec3 tombPos[6];

glm::mat4 proj;
glm::vec3 light = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::mat4 rotP = glm::rotate(0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotN = glm::rotate(-0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
float enemyDist = 2.0f;
float factor = 0.2f;
int tombCount = 5;
const int maxTombCount = tombCount;

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

	trees = new Object* [400];
	int seed = 713;
	float posx = -1.0f, posz = -1.0f;
	trees[0] = new Object;
	trees[0]->loadData("tree.obj", "pine_tree.ktx");
	trees[0]->bindProgram(height_program);
	trees[0]->scale(0.015f);
	trees[0]->scale(5.0f); // 3.0f
	for(int i = 1; i < 400; i++){
		trees[i] = new Object(*trees[0]);
		seed = (seed * 33 + 13)%1024; //4096; 
		posx = ((seed >> 5) - 16)/16.0f;
		posz = (seed % 32 - 16)/16.0f;
		trees[i]->scale(1.0f, 1.0f + 0.1f * (i%6), 1.0f);
		trees[i]->translate(posx, 0.0f, posz);
	}

	tomb = new Object* [maxTombCount+1];
	tomb[0] = new Object;
	tomb[0]->loadData("tombstone.obj", "tombstone.ktx");
	tomb[0]->bindProgram(height_program);
	tomb[0]->scale(0.03f);
	for(int i = 1; i < maxTombCount+1; i++){
		tomb[i] = new Object(*tomb[0]);
		seed = (seed * 3 + i)%1024; //4096; 
		posx = ((seed >> 5) - 16)/16.0f;
		posz = (seed % 32 - 16)/16.0f;
		tomb[i]->scale(1.0f, 1.0f + 0.1f * (i%6), 1.0f);
		tomb[i]->rotate(0, 1, 0, seed%90);
		tomb[i]->translate(posx, 0.0f, posz);
		tombPos[i-1] = glm::vec3(posx, posz, 1.0f);
	}

	slender = new Enemy;
	slender->loadData("iron_man.obj", "iron_man.ktx");
	slender->bindProgram(height_program);
	slender->scale(0.01f, 0.01f, 0.01f);
	slender->translate(0.9f, 0.05f, 0.9f);

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

		static float vertical_angle = 0.0f;

		if(Input::instance().moveMouseY()){
			c->rotate(1, 0, 0, -0.01f * Input::instance().moveMouseY());
			vertical_angle += -0.01f * Input::instance().moveMouseY();
		}
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

		if(Input::instance().isJustPressedKey(GLFW_KEY_X)){
			for(int i = 0; i < maxTombCount; i++){
				if(tombPos[i].z > 0.0f){
					if(glm::distance(glm::vec2(tombPos[i].x, tombPos[i].y), glm::vec2(me->x(), me->z())) < 0.05f){
						tombPos[i].z = -1.0f;
						factor *= 1.5f;
						tombCount--;
						break;
					}	
				}
			}
		}

		if(glfwJoystickPresent(GLFW_JOYSTICK_1)){

			Input::instance().updateJoyButton();
			Input::instance().updateJoyAxes();


			if(Input::instance().moveJoyAxis2Y()){
				c->rotate(1, 0, 0, -0.01f * Input::instance().moveJoyAxis2Y()*2);
				vertical_angle += -0.01f * Input::instance().moveJoyAxis2Y()*2;
			}
			if(Input::instance().moveJoyAxis2X()){
				if(vertical_angle){
					c->rotate(1, 0, 0, -vertical_angle);
					c->rotate(0, 1, 0, -0.01f * Input::instance().moveJoyAxis2X()*3);
					c->rotate(1, 0, 0, vertical_angle);
				}
				else{
					c->rotate(0, 1, 0, -0.01f * Input::instance().moveJoyAxis2X()*3);
				}
			}

			if(Input::instance().isJustPressedJoyButton(JOY_CIRCLE))
				glfwSetWindowShouldClose(window, GL_TRUE);

			if(Input::instance().isJustPressedJoyButton(JOY_CROSS)){
				for(int i = 0; i < maxTombCount; i++){
					if(tombPos[i].z > 0.0f){
						if(glm::distance(glm::vec2(tombPos[i].x, tombPos[i].y), glm::vec2(me->x(), me->z())) < 0.05f){
							tombPos[i].z = -1.0f;
							factor *= 1.5f;
							tombCount--;
							break;
						}	
					}
				}
			}			
		}


		lastTime = glfwGetTime();

		float desat = enemyDist*4.0f;
		desat = desat > 1.0f ? 1.0f : desat;
		desat = 1-desat;
		desat *= desat;

		float bw = (0.5294f + 0.8078f + 0.9804f)/3.0f;
		const GLfloat background_color[] = {(bw-0.5294f)*desat + 0.5294f, (bw-0.8078f)*desat + 0.8078f, (bw-0.9804f)*desat + 0.9804f, 1.0f};

		// const GLfloat background_color[] = {0.5294f, 0.8078f , 0.9804f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);

		GLfloat far = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &far);

		glUseProgram(ter_program);
		c->bindProgram(ter_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		glUniform1f(7, desat);
		t->draw();

		glUseProgram(grass_program);
		c->bindProgram(grass_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		glUniform1f(7, desat);


		static bool grassOK = true;
		if(Input::instance().isJustPressedKey(GLFW_KEY_G)) grassOK = !grassOK;
		if(grassOK) grass->draw();

		glUseProgram(height_program);
		c->bindProgram(height_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		glUniform1f(7, desat);
		for(int i = 0; i < 150; i++){
			if(trees[i] != NULL)
				trees[i]->draw();
		}
		me->draw();

		for(int i = 1; i < maxTombCount+1; i++){
			if(tomb[i] != NULL && tombPos[i-1].z > 0.0f){
				tomb[i]->draw();
			}
		}

		if(tombCount){
			glUniform1f(7, 0.0f);
			slender->draw(me->x(), me->z(), factor, &enemyDist);
		}
		else{
			enemyDist = 2.0f;
		}

		if(enemyDist <= 0.001f) glfwSetWindowShouldClose(window, GL_TRUE);

		Input::instance().resetMouse();
		Input::instance().resetJoyAxes();
		Input::instance().updateKeys();
		
		//glfwSwapInterval(1);
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
	
	delete c;
	delete t;
	delete plane;
	delete[] trees;
	delete me;
	delete grass;
	delete slender;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}