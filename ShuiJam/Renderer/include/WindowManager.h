/*\file WindowManager.h*/
#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>

class WindowManager
{
private:
	GLFWwindow* m_window = nullptr;
	int m_width, m_height, m_vsync;
	bool m_fullscreen;
	std::string m_title;
	void Initialise();

public:
	WindowManager(int width, int height, int vsync, bool fullscreen, std::string title) : m_width(width), m_height(height), m_vsync(vsync), m_fullscreen(fullscreen), m_title(title) { Initialise(); }

	void Shutdown();
	void Loop();
	void Start();
	void CloseWindow();
	inline GLFWwindow* getWindow() { return m_window; }
};
