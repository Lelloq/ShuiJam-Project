#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class WindowManager
{
private:
	GLFWwindow* m_window = nullptr;
	int m_width, m_height, m_vsync;
	std::string m_title;
	void Initialise();

public:
	WindowManager(int width, int height, int vsync, std::string title) : m_width(width), m_height(height), m_vsync(vsync), m_title(title) { Initialise(); }

	void Shutdown();
	void Loop();
	void Start();
	void CloseWindow();
	inline GLFWwindow* getWindow() { return m_window; }
};