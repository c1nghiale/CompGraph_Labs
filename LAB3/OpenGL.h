#pragma once

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "Event.h"

struct MouseWheelEventArg
{
	double value;
};

struct MouseButtonEventArg
{
	int button;
	int action;
};

struct MouseEventArg
{
	short x;
	short y;
};

struct KeyEventArg
{
	int key;
	int action;
};


class OpenGL
{
	bool render_flag;

	int m_window_width;
	int m_window_height;

	int m_framebuffer_width;
	int m_framebuffer_height;

	GLFWwindow* m_window;

	Event<OpenGL*, const MouseEventArg&> m_mouseMoveEvent;
	Event<OpenGL*, const MouseWheelEventArg&> m_wheelEvent;
	Event<OpenGL*, const MouseButtonEventArg&> m_mouseButtonEvent;
	Event<OpenGL*, const KeyEventArg&> m_keyEvent;

	static void window_size_callback(GLFWwindow* window, int width, int height);

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void PreRender();
	void resize();

	void drawAxisAndCell();
	
	double old_time = 0;

public:

	
	auto& mouseMoveEvent() { return m_mouseMoveEvent; }
	auto& wheelEvent() { return m_wheelEvent; }
	auto& mouseButtonEvent (){ return m_mouseButtonEvent; }
	auto& keyEvent() { return m_keyEvent; }

	void initWindow();


	

	void Render();

	

};