#ifndef TEXTURE_CONTROL_H
#define TEXTURE_CONTROL_H

#include <GLFW/glfw3.h>

class TextureControl{
public:
	static TextureControl& instance();
	TextureControl(TextureControl const& copy) = delete;
	~TextureControl();
    void operator=(TextureControl const& copy) = delete;
    GLenum retriveUnit();
private:
	TextureControl();
	int count = 0;
	int maxUnits;
};

#endif