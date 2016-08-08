#pragma once

#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <iostream>
#include "OGLUtility.hpp"

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

	switch (key)
	{
	case GLFW_KEY_0:
		fov = 45.0f;
		nearDistance = 0.1f;
		farDistance = 100.0f;
		break;
	case GLFW_KEY_1:
		fov += 1.0f;
		break;
	case GLFW_KEY_2:
		fov -= 1.0f;
		break;
	case GLFW_KEY_4:
		nearDistance += 0.1f;
		break;
	case GLFW_KEY_5:
		nearDistance -= 1.0f;
		break;
	case GLFW_KEY_7:
		farDistance += 1.0f;
		break;
	case GLFW_KEY_8:
		farDistance -= 1.0f;
		break;
	default:
		break;
	}

	if (key >= GLFW_KEY_0 && key <= GLFW_KEY_8)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		projection = glm::perspective(fov, width / (float)height, nearDistance, farDistance);
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
	projection = glm::perspective(fov, width / (float)height, nearDistance, farDistance);
}
