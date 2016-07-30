#pragma once

#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <iostream>

static void OnGLFWError(int errorCode, const char* description)
{
	std::cerr << "GLFW error code: " << errorCode << " info: " << description << std::endl;
}

static void OnMouseButtonClick(GLFWwindow* window, int button, int action, int mods)
{
    TwEventMouseButtonGLFW(button, action);
}

static void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    TwMouseMotion(int(xpos), int(ypos));
}

static void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    TwEventMouseWheelGLFW((int)yoffset);
}

static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    TwEventKeyGLFW(key, action);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void OnCharPressed(GLFWwindow* window, unsigned int codepoint)
{
    TwEventCharGLFW(codepoint, GLFW_PRESS);
}

static void OnWindowResize(GLFWwindow* window, int width, int height)
{
    TwWindowSize(width, height);
    glViewport(0, 0, width, height);
}
