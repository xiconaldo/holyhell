#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

class Input{
public:
	static Input& instance();
	Input(Input const& copy) = delete;
	~Input();
    void operator=(Input const& copy) = delete;

	void update(int key, int action);
	bool isJustPressed(int key);
	bool isReleased(int key);
	bool isPressed(int key);
	int  getState(int key);

	void update(double xpos, double ypos);
	double moveX();
	double moveY();
	void reset();

private:
	Input();
	int *keyboard;
	double dx;
	double dy;
};

#endif