/*\file Scene.h
\brief Base scene that contains the key input functions*/
#pragma once
#include <GLFW/glfw3.h>

namespace SJ
{
	/*\class Scene
	\brief Base scene that holds input functions and static functions*/
	class Scene
	{
	private:
		inline static GLFWwindow* s_window;
	public:
		~Scene() = default;

		virtual void getKey(int key, int scancode, int action, int mods) = 0;
		virtual void getMouseButton(int button, int action, int mods) = 0;
		virtual void getScroll(double xoffset, double yoffset) = 0;
		virtual void fileDrop(int count, const char** paths) = 0;

		static void setWindow(GLFWwindow* window)
		{
			Scene::s_window = window;
		}

		static void setInputCallbacks(Scene& scene)
		{
			glfwSetWindowUserPointer(s_window, &scene);

			glfwSetKeyCallback(s_window, [](GLFWwindow* win, int key, int scancode, int action, int mods)
				{
					static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->getKey(key, scancode, action, mods);
				});

			glfwSetMouseButtonCallback(s_window, [](GLFWwindow* win, int button, int action, int mods)
				{
					static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->getMouseButton(button, action, mods);
				});

			glfwSetScrollCallback(s_window, [](GLFWwindow* win, double xoffset, double yoffset)
				{
					static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->getScroll(xoffset, yoffset);
				});

			glfwSetDropCallback(s_window, [](GLFWwindow* win, int count, const char** paths)
				{
					static_cast<decltype(&scene)>(glfwGetWindowUserPointer(win))->fileDrop(count, paths);
				});
		}
	};
}