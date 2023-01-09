/*\file Scene.h
\brief Base scene that contains the key input functions*/
#pragma once

namespace SJ
{
	/*\class Scene
	\brief Base scene that holds input functions*/
	class Scene
	{
	public:
		~Scene() = default;

		virtual void getKey(int key, int scancode, int action, int mods) = 0;
		virtual void getMouseButton(int button, int action, int mods) = 0;
		virtual void getScroll(double xoffset, double yoffset) = 0;
		virtual void fileDrop(int count, const char** paths) = 0;
	};
}