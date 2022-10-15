#include "pch.h"
#include "WindowManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, 1920, 1080); };

void WindowManager::Initialise()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, "ShuiJam", NULL, NULL);
	glfwMakeContextCurrent(m_window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	glfwSwapInterval(m_vsync);
}

void WindowManager::Shutdown()
{
	glfwTerminate();
	glfwDestroyWindow(m_window);
}

void WindowManager::Loop()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}