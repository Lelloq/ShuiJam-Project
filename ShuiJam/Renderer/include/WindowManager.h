#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class WindowManager
{
private:
	GLFWwindow* m_window = nullptr;
	int m_width, m_height, m_vsync;
	void Initialise();

public:
	WindowManager(int width, int height, int vsync) : m_width(width), m_height(height), m_vsync(vsync) { Initialise(); }

	void Shutdown();
	void Loop();
	void Start();
	inline GLFWwindow* getWindow() { return m_window; }
};