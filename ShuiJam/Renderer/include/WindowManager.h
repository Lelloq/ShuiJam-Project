/*\file WindowManager.h*/
#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include <wtypes.h>

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

		float m_dt = 0.f;//!< Delta time 
		float m_currentFrame = 0.f;//!<Current frame time
		float m_lastFrame = 0.f;//!<Previous frame time
	public:
		WindowManager(int width, int height, int vsync, bool fullscreen, std::string title) 
			: m_width(width), m_height(height), m_vsync(vsync), m_fullscreen(fullscreen), m_title(title) { Initialise(); } //!< initialises glfw with desired properties in members

		void Shutdown(); //!< Void shutdown terminates glfw
		void Swap(); //!< Updates the current window
		void CloseWindow(); //!< Closes glfw window
		void beginFrame();//Start the frame and clear colour and depth buffer
		inline GLFWwindow* getWindow() { return m_window; } //!< GLFW window getter
		inline float getDeltatime() { return m_dt; }//!< Get delta time
	};
}
