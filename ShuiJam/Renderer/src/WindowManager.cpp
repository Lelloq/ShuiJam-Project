/*\file WindowManager.cpp*/
#include "WindowManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); };

//Initialises glfw with latest version
void WindowManager::Initialise()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

//Creates window, sets vsync, width, height, title and fullscreen
void WindowManager::Start()
{
	if(m_fullscreen) {m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), glfwGetPrimaryMonitor(), NULL); }
	else { m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL); }
	glfwMakeContextCurrent(m_window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	glfwSwapInterval(m_vsync);
}

//Termines glfw and destroys window
void WindowManager::Shutdown()
{
	glfwTerminate();
	glfwDestroyWindow(m_window);
}

//Closes window without terminating glfw
void WindowManager::CloseWindow()
{
	glfwDestroyWindow(m_window);
}

//Swaps buffers - will change name soon it's a bit misleading
void WindowManager::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}
