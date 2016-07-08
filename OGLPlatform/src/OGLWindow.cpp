#include "OGLWindow.hpp"
#include <iostream>

OGLWindow::OGLWindow()
{
}

OGLWindow::~OGLWindow()
{
	glfwDestroyWindow(window);
}

bool OGLWindow::CreateWindow(int width, int height, const std::string& title)
{
	bool res = false;

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	res = (window != nullptr);
    this->width = width;
    this->height = height;
    
	return res;
}