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
	Variant var;
	var.asDouble = mousePosX;
	mouseEvent.AddArgument(gs::Event::MOUSEX, var);
	var.asDouble = mousePosY;
	mouseEvent.AddArgument(gs::Event::MOUSEY, var);
	var.asInteger = mouseState;
	mouseEvent.AddArgument(gs::Event::BUTTON_STATE, var);
	var.asInteger = mouseButton;
	mouseEvent.AddArgument(gs::Event::BUTTON, var);

	gs::EventManager::GetInstance()->QueueEvent(mouseEvent);
}

static void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	mousePosX = xpos;
	mousePosY = ypos;

	gs::Event mouseEvent(EventType::ET_MOUSE_MOVED);
	Variant var;
	var.asDouble = mousePosX;
	mouseEvent.AddArgument(gs::Event::MOUSEX, var);
	var.asDouble = mousePosY;
	mouseEvent.AddArgument(gs::Event::MOUSEY, var);
	var.asInteger = mouseState;
	mouseEvent.AddArgument(gs::Event::BUTTON_STATE, var);
	var.asInteger = mouseButton;
	mouseEvent.AddArgument(gs::Event::BUTTON, var);

	gs::EventManager::GetInstance()->QueueEvent(mouseEvent);
}

static void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    TwEventMouseWheelGLFW((int)yoffset);
	// Add mouse wheel event to EventManager queue
}

static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int width, height = 0;
	glfwGetWindowSize(window, &width, &height);

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
		gs::Event keyEvent(EventType::ET_KEY_PRESSED);
		Variant var;
		var.asInteger = key;
		keyEvent.AddArgument(gs::Event::KEY, var);
		var.asInteger = width;
		keyEvent.AddArgument(gs::Event::WIDTH, var);
		var.asInteger = height;
		keyEvent.AddArgument(gs::Event::HEIGHT, var);
		gs::EventManager::GetInstance()->QueueEvent(keyEvent);
    }
	else if (action == GLFW_RELEASE) {
		gs::Event keyEvent(EventType::ET_KEY_RELEASED);
		Variant var;
		var.asInteger = key;
		keyEvent.AddArgument(gs::Event::KEY, var);
		var.asInteger = width;
		keyEvent.AddArgument(gs::Event::WIDTH, var);
		var.asInteger = height;
		keyEvent.AddArgument(gs::Event::HEIGHT, var);
		gs::EventManager::GetInstance()->QueueEvent(keyEvent);
	}
}

static void OnCharPressed(GLFWwindow* window, unsigned int codepoint)
{
    TwEventCharGLFW(codepoint, GLFW_PRESS);
}

static void OnWindowResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    //gs::EventManager::GetInstance().QueueEvent(gs::EventType::ET_WINDOW_RESIZED, new gs::ResizeEventArgs(width, height));
}
