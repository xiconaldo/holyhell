#ifndef SHADER_UTIL_H
#define SHADER_UTIL_H

#include <GL/glew.h>
#include <loader.h>
#include <iostream>

/**
 * Conjunto de funções usadas para compilar e linkar shaders de maneira mais simples.
 *
 * TODO:
 * 		+ Função que receba um bitfield indicando quais estágios
 * 		os shaders representam e um conjunto de nomes de arquivos 
 * 		contendo o código fonte do shader. Retorna um símbolo para 
 * 		um programa já linkado e apaga os objetos intermediários
 * 		criados.
 *   	- Função semelhante a anterior, mas que gera um pipeline
 *   	program.
 *   	+ Interessante incluir uma variável capaz de guardar o endereço 
 *   	base do diretório onde ficam os shaders para reduzir os caminhos
 *   	(sugestão: usar functors).
 */

//GLuint create_program(GLint shaderCount, GLenum *shaderType, std::string *location);

class CreateProgram{
public:
	CreateProgram(){}
	CreateProgram(const std::string& location);
	GLuint operator()(GLint shaderCount, GLenum *shaderType, std::string *location);
	std::string baseLocation;
};

#endif