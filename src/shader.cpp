#include <shader.h>

/**
 * Construtor
 * @param  location 	endereço base onde encontram-se os shaders.
 */
CreateProgram::CreateProgram(const std::string& location) :
	baseLocation(location)
{}

/**
 * Gera um programa a partir de shaders fornecidos.
 * @param  shaderCount número de shaders que estarão presentes no programa.
 * @param  shaderType  array conteúdo os tipos de shader que farão parte do programa.
 * @param  location    array conteúdo os respectivos endereços em disco dos códigos-
 *                     fonte dos shaders a serem usados.
 * @return             retorna um número que representa o programa criado.
 */
GLuint CreateProgram::operator()(GLint shaderCount, GLenum *shaderType, std::string *location){
	GLuint *shader = new GLuint[shaderCount];
	GLint program, success, log_size;
	std::string shader_source;
	GLchar *log;

	program = glCreateProgram();

	for(int i = 0; i < shaderCount; i++){
		shader[i] = glCreateShader(shaderType[i]);
		shader_source = load_shader(baseLocation + location[i]);
		const GLchar *sources[] = {shader_source.c_str()};

		glShaderSource(shader[i], 1, sources, NULL);
		glCompileShader(shader[i]);
		glGetShaderiv(shader[i], GL_COMPILE_STATUS, &success);

		if(success == GL_FALSE){
			glGetShaderiv(shader[i], GL_INFO_LOG_LENGTH, &log_size);
			log = new GLchar[log_size];
			glGetShaderInfoLog(shader[i], log_size, NULL, log);
			std::cout << location[i].substr(location[i].rfind('/')+1) << " failed compilation:" << std::endl;
			std::cout << log << std::endl;
			delete[] log;
			exit(EXIT_FAILURE);
		}

		glAttachShader(program, shader[i]);
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	
	if(success == GL_FALSE){
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
		log = new GLchar[log_size];
		glGetProgramInfoLog(program, log_size, NULL, log);
		std::cout << "Program " << program << " failed linkage:" << std::endl;
		std::cout << log << std::endl;
		delete[] log;
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < shaderCount; i++){
		glDetachShader(program, shader[i]);
		glDeleteShader(shader[i]);
	}
	
	return program;
}