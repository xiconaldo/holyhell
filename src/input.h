#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

enum {JOY_TRIANGLE = 0, JOY_CIRCLE, JOY_CROSS, JOY_SQUARE, JOY_L1, JOY_R1, JOY_L2, JOY_R2, JOY_BACK, JOY_START, JOY_L3, JOY_R3};
enum {JOY_Y = 0, JOY_B, JOY_A, JOY_X, JOY_LB, JOY_RB, JOY_LT, JOY_RT, JOY_VIEW, JOY_MENU, JOY_LS, JOY_RS};

class Input{
public:
	static Input& instance();
	Input(Input const& copy) = delete;
	~Input();
    void operator=(Input const& copy) = delete;

	void updateKey(int key, int action);
	void updateKeys();
	bool isJustPressedKey(int key);
	bool isReleasedKey(int key);
	bool isPressedKey(int key);
	int  getStateKey(int key);

	void updateMouseMotion(double xpos, double ypos);
	void updateMouseButton(int button, int action);
	bool isJustPressedButton(int button);
	bool isReleasedButton(int button);
	bool isPressedButton(int button);
	int  getStateButton(int button);
	double moveMouseX();
	double moveMouseY();
	void resetMouse();

	void updateJoyButton();
	bool isJustPressedJoyButton(int key);
	bool isReleasedJoyButton(int key);
	bool isPressedJoyButton(int key);
	int  getStateJoyButton(int key);

	void updateJoyAxes();
	double moveJoyAxis1X();
	double moveJoyAxis1Y();
	double moveJoyAxis2X();
	double moveJoyAxis2Y();
	void resetJoyAxes();

private:
	Input();
	int *keyboard;
	int *mouse;
	unsigned char *joystickButtons;
	double mouse_dx;
	double mouse_dy;
	double axis1_dx;
	double axis1_dy;
	double axis2_dx;
	double axis2_dy;
};

#endif