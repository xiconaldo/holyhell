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
Camera *light_cam;
Terrain *t;
Object *plane;
Object *tree, *tree2, *tree3, *stark;
Object **trees;
Player *me;
Grass *grass;
glm::mat4 proj;
glm::mat4 light_proj;
glm::vec4 light = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
glm::mat4 rotP = glm::rotate(0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 rotN = glm::rotate(-0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
int numArvores = 100;

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

	std::string shadow_h_names[] = {"shadow_height_vertex.glsl", "shadow_frag.glsl"};
	GLuint shadow_height_program = create_program(2, flags, shadow_h_names);

	std::string shadow_g_names[] = {"shadow_grass_vertex.glsl", "shadow_frag.glsl"};
	GLuint shadow_grass_program = create_program(2, flags, shadow_g_names);

	std::string shadow_ter_names[] = {"terrain_vertex.glsl", "terrain_tess_ct.glsl", "shadow_terrain_tess_ev.glsl", "shadow_frag.glsl"};
	GLuint shadow_ter_program = create_program(4, ter_flags, shadow_ter_names);


	std::string bosta[] = {"coco_v.glsl", "coco_f.glsl"};	
	GLuint cocozinho = create_program(2, flags, bosta);

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
	trees[0]->scale(3.0f);
	for(int i = 1; i < numArvores; i++){
		trees[i] = new Object(*trees[0]);
		seed = (seed * 3 + i)%1024; //4096; 
		posx = ((seed >> 5) - 16)/16.0f;
		posz = (seed % 32 - 16)/16.0f;
		trees[i]->scale(1.0f, 1.0f + 0.1f * (i%6), 1.0f);
		trees[i]->translate(posx, -0.01f, posz);
	}

	c = new Camera(0, 0, 0, me->x(), 0.0f, me->z());
	light_cam = c;
	light_cam = new Camera(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	proj = glm::infinitePerspective(3.14f/4.0f, 16.0f/9.0f, 0.001f);
	proj = glm::perspective(45.0f, 16.0f/9.0f, 0.001f, 1.5f);
	light_proj = glm::ortho(-1.4f,1.4f,-0.9f,1.1f,0.7f,3.0f);
	//light_proj = proj;
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0);



	/////////////////////
	// SHADOW MAP DRAW //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 576, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);


	///////////////
	// Main Loop //
	///////////////
	
	double lastTime, currentTime;
	int frame = 0;

	while (!glfwWindowShouldClose(window))
	{
		///////////////////
		// INPUT CONTROL //
		/////////////////////////////////////////////////////////////////////////////////

		static float vertical_angle = 0.0f;

		static bool grassOK = false;
		if(Input::instance().isJustPressedKey(GLFW_KEY_G)) grassOK = !grassOK;

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
		}

		lastTime = glfwGetTime();

		////////////////////////////////////////////////////////////////////////////////////////////
		


		/////////////////////////
		// DEPTH BUFFER RENDER //
		///////////////////////////////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		GLfloat far = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &far);

		glUseProgram(shadow_ter_program);
		light_cam->bindProgram(shadow_ter_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(light_proj));
		glUniform3fv(3, 1, glm::value_ptr(light));
		t->draw();

		glUseProgram(shadow_grass_program);
		light_cam->bindProgram(shadow_grass_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(light_proj));
		// grass->draw();

		glUseProgram(shadow_height_program);
		light_cam->bindProgram(shadow_height_program);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(light_proj));
		for(int i = 0; i < numArvores; i++){
			if(trees[i] != NULL)
				trees[i]->draw();
		}
		//me->draw();
		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(cocozinho);

		GLuint myVAO;
		glGenVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);

		GLfloat quad[] = {-1.0f, 1.0f,  -1.0f, -1.0f,  1.0f, -1.0f,
						  1.0f, 1.0f,  -1.0f,  1.0f,  1.0f, -1.0f};

		GLfloat tex[] = {0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f,
					     1.0f, 1.0f,  0.0f, 1.0f,  1.0f, 0.0f};

		GLuint drawBuffer[2];
		glGenBuffers(2, drawBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, drawBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(GLfloat), (void*)quad, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, drawBuffer[1]);
		glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(GLfloat), (void*)tex, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthTexture);

		const GLfloat background_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		////////////////////////////////////////////////////////////////////////////////////



		/////////////////
		// NORMAL DRAW //
		////////////////////////////////////////////////////////////////////////////////////
		
		// glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// const GLfloat background_color[] = {0.5294f, 0.8078f , 0.9804f, 1.0f};
		// glEnable(GL_DEPTH_TEST);
		// glClearBufferfv(GL_COLOR, 0, background_color);
		// glClearBufferfv(GL_DEPTH, 0, &far);

		// glUseProgram(ter_program);
		// c->bindProgram(ter_program);
		// glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		// glUniform3fv(3, 1, glm::value_ptr(light));
		// glUniformMatrix4fv(8, 1, GL_FALSE, glm::value_ptr(light_cam->getView()));
		// glUniformMatrix4fv(9, 1, GL_FALSE, glm::value_ptr(light_proj));
		// glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(biasMatrix));
		// t->draw();

		// glUseProgram(grass_program);
		// c->bindProgram(grass_program);
		// glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		// glUniform3fv(3, 1, glm::value_ptr(light));
		// glUniformMatrix4fv(8, 1, GL_FALSE, glm::value_ptr(light_cam->getView()));
		// glUniformMatrix4fv(9, 1, GL_FALSE, glm::value_ptr(light_proj));
		// glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(biasMatrix));

		// if(grassOK)
		// grass->draw();

		// glUseProgram(height_program);
		// c->bindProgram(height_program);
		// glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(proj));
		// glUniformMatrix4fv(8, 1, GL_FALSE, glm::value_ptr(light_cam->getView()));
		// glUniformMatrix4fv(9, 1, GL_FALSE, glm::value_ptr(light_proj));
		// glUniformMatrix4fv(10, 1, GL_FALSE, glm::value_ptr(biasMatrix));

		// for(int i = 0; i < numArvores; i++){
		// 	if(trees[i] != NULL)
		// 		trees[i]->draw();
		// }
		// me->draw();

		///////////////////////////////////////////////////////////////////////////////////
		

		///////////
		// EXTRA //
		///////////

		Input::instance().resetMouse();
		Input::instance().resetJoyAxes();
		Input::instance().updateKeys();
		
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