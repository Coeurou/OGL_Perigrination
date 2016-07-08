#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include "Uncopyable.hpp"

class OGLWindow : public Uncopyable
{
public:
	OGLWindow();
	~OGLWindow();

	bool CreateWindow(int width, int height, const std::string& title);
	GLFWwindow* get() const { return window; }
    int GetWindowWidth() const { return width; }
    int GetWindowHeight() const { return height;}

private:
	GLFWwindow* window;
    int width;
    int height;
};

