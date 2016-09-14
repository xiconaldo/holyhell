#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <loader.h>


static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


//Compila os shaders
GLuint compile_shaders(const std::string& vertex_shader_location, const std::string& frag_shader_location){
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
 
 	std::string vertex_shader_string = load_shader(vertex_shader_location);
 	std::string fragment_shader_string = load_shader(frag_shader_location);
	const GLchar *vertex_shader_source[] = {vertex_shader_string.c_str()};
	const GLchar *fragment_shader_source[] = {fragment_shader_string.c_str()};

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	GLint success;

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_TRUE){ 
		std::cout << "Success compilation vertex_shader!" << std::endl;
	}else{
		GLint log_length;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
		GLchar * str = (GLchar*) malloc(log_length);
		glGetShaderInfoLog(vertex_shader, log_length, NULL, str);
		std::cout << str << std::endl;
	}


	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);


	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_TRUE){ 
		std::cout << "Success compilation fragment_shader!" << std::endl;
	}else{
		GLint log_length;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *str = (GLchar*) malloc(log_length);
		glGetShaderInfoLog(fragment_shader, log_length, NULL, str);
		std::cout << str << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
	glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

// Compila shaders

int main(int argc, const char* argv[]){

	std::string location(argv[0]);
	location = location.substr(0, location.rfind('/')+1);

	// Inicialização do GLFW e GLEW
	//#############################################################
	
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	
	window = glfwCreateWindow(600, 600, "Projeto de Computação Gráfica", NULL, NULL);
	glfwSetWindowPos(window, 200, 10);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	//#############################################################

	//#############################################################
	// Parte OpenGL

	GLuint rendering_program = compile_shaders(location + "../shaders/vertex_shader_source.glsl",
											   location + "../shaders/fragment_shader_source.glsl");

	GLint success; 
	glGetProgramiv(rendering_program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
			std::cout << "Erro de Linkagem" << std::endl;
	else
		std::cout << "Success linkage!" << std::endl;

	glUseProgram(rendering_program);


	GLint tr;
	Group *g;
	load_grouped_data(location + "../data/man2.obj", tr, &g);
	tr *= 3;
	glPointSize(10.0f);
	//################################################################

	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	GLuint vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, tr*sizeof(Group), (void*)g, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{

		const GLfloat background_color[] = {0.5f, 0.5f , 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, background_color); // limpar a tela
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Group), NULL);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Group), (void*)offsetof(Group, normals));
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, tr);

		glfwSwapBuffers(window);
		glfwPollEvents(); // última instrução no while

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	//##################################################################
}