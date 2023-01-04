/*\file WindowManager.h*/
#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>

namespace SJ
{
	class WindowManager
	{
	private:
		GLFWwindow* m_window = nullptr; //!< GLFW window pointer initialised as nullptr
		int m_width, m_height, m_vsync; //!< int width, height and vsync
		bool m_fullscreen; //!< bool is fullscreen member
		std::string m_title; //!< title
		void Initialise(); //!< private void Initialise() function

	public:
		WindowManager(){};//!< Default constructor
		WindowManager(int width, int height, int vsync, bool fullscreen, std::string title) 
			: m_width(width), m_height(height), m_vsync(vsync), m_fullscreen(fullscreen), m_title(title) { Initialise(); } //!< initialises glfw with desired properties in members

		void Shutdown(); //!< Void shutdown terminates glfw
		void Update(); //!< Updates the current window
		void Start(); //!< Starts GLFW window
		void CloseWindow(); //!< Closes glfw window
		inline GLFWwindow* getWindow() { return m_window; } //!< GLFW window getter
	};
}
