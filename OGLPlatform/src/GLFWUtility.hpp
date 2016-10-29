#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <iostream>
#include "OGLUtility.hpp"
#include "Event.hpp"
#include "EventManager.hpp"

static bool polygonState = false;
static int mouseButton = 0;
static int mouseState = 0;
static double mousePosX = 0;
static double mousePosY = 0;

static void OnGLFWError(int errorCode, const char* description)
{
    std::cerr << "GLFW error code: " << errorCode << " info: " << description << std::endl;
}

static void OnMouseButtonClick(GLFWwindow* window, int button, int action, int mods)
{
	mouseButton = button;
    mouseState = action;

	gs::Event mouseEvent(EventType::ET_MOUSE_PRESSED);
	mouseEvent.args.mousePosX = mousePosX;
	mouseEvent.args.mousePosY = mousePosY;
	mouseEvent.args.mouseButton = mouseButton;
	mouseEvent.args.mouseButtonState = mouseState;

	gs::EventManager::GetInstance()->QueueEvent(mouseEvent);
}

static void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	mousePosX = xpos;
	mousePosY = ypos;

	gs::Event mouseEvent(EventType::ET_MOUSE_MOVED);
	mouseEvent.args.mousePosX = mousePosX;
	mouseEvent.args.mousePosY = mousePosY;
	mouseEvent.args.mouseButton = mouseButton;
	mouseEvent.args.mouseButtonState = mouseState;

	gs::EventManager::GetInstance()->QueueEvent(mouseEvent);
}

static void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    TwEventMouseWheelGLFW((int)yoffset);
	// Add mouse wheel event to EventManager queue
}

static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_Z:
                if (polygonState) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                polygonState = !polygonState;
                
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                break;
        }
    }
	gs::Event keyEvent(EventType::ET_KEY);
	keyEvent.args.key = key;
	keyEvent.args.keyState = action;
	gs::EventManager::GetInstance()->QueueEvent(keyEvent);
}

static void OnCharPressed(GLFWwindow* window, unsigned int codepoint)
{
    TwEventCharGLFW(codepoint, GLFW_PRESS);
}

static void OnWindowResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

	gs::Event resizeEvent(EventType::ET_WINDOW_RESIZED);
	resizeEvent.args.width = width;
	resizeEvent.args.height = height;
	gs::EventManager::GetInstance()->QueueEvent(resizeEvent);
}

static void OnFrameBufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
