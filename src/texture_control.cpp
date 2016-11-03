#include <texture_control.h>

TextureControl& TextureControl::instance(){
	static TextureControl instance;
	return instance;
}

TextureControl::TextureControl(){
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
}

TextureControl::~TextureControl(){}

GLenum TextureControl::retriveUnit(){
	
	GLenum aux = GL_TEXTURE0 + count;
	count = ++count % maxUnits;


	return aux;
}