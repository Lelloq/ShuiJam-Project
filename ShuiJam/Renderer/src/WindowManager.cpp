/*\file WindowManager.cpp*/
#include "WindowManager.h"

namespace SJ
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); };

	//Initialises glfw with latest version, sets vsync, width, height, title and fullscreen
	void WindowManager::Initialise()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		if (m_fullscreen) { m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), glfwGetPrimaryMonitor(), NULL); }
		else
		{
			m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
			glfwSetWindowPos(m_window, GetSystemMetrics(SM_CXSCREEN) / 2 - m_width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - m_height / 2);
		}
		glfwMakeContextCurrent(m_window);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		glfwSwapInterval(m_vsync);

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//Termines glfw and destroys window
	void WindowManager::Shutdown()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	//Closes window without terminating glfw
	void WindowManager::CloseWindow()
	{
		glfwDestroyWindow(m_window);
	}

	//Begins frame and clears buffer bits
	void WindowManager::beginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_currentFrame = glfwGetTime();
		m_dt = m_currentFrame - m_lastFrame;
		m_lastFrame = m_currentFrame;
	}

	//Swaps buffers
	void WindowManager::Swap()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}
