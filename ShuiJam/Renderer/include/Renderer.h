#pragma once
#include <glad/glad.h>

class Renderer
{
public:
	void Draw() const
	{

	};

	//Clears the window
	void Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
};