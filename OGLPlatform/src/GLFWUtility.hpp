#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <iostream>
#include "OGLUtility.hpp"
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
	gs::EventManager::GetInstance().QueueEvent(gs::EventType::ET_MOUSE_PRESSED, new gs::MouseEventArgs(mouseButton, mouseState, mousePosX, mousePosY));
}

static void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	mousePosX = xpos;
	mousePosY = ypos;
    gs::EventManager::GetInstance().QueueEvent(gs::EventType::ET_MOUSE_MOVED, new gs::MouseEventArgs(mouseButton, mouseState, mousePosX, mousePosY));
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
	int width, height = 0;
	glfwGetWindowSize(window, &width, &height);
	gs::EventManager::GetInstance().QueueEvent(gs::EventType::ET_KEY_PRESSED, new gs::KeyEventArgs(key, width, height));
}

static void OnCharPressed(GLFWwindow* window, unsigned int codepoint)
{
    TwEventCharGLFW(codepoint, GLFW_PRESS);
}

static void OnWindowResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    gs::EventManager::GetInstance().QueueEvent(gs::EventType::ET_WINDOW_RESIZED, new gs::ResizeEventArgs(width, height));
}
