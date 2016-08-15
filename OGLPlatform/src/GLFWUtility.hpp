#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <iostream>
#include "OGLUtility.hpp"
#include "EventManager.hpp"

bool polygonState = false;
int mouseState = 0;

static void OnGLFWError(int errorCode, const char* description)
{
    std::cerr << "GLFW error code: " << errorCode << " info: " << description << std::endl;
}

static void OnMouseButtonClick(GLFWwindow* window, int button, int action, int mods)
{
    TwEventMouseButtonGLFW(button, action);
    mouseState = (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
}

static void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    TwMouseMotion(int(xpos), int(ypos));
    
    gs::EventManager::GetInstance().Dispatch(gs::EventType::ET_MOUSE_MOVED, gs::MouseEventArgs(GLFW_MOUSE_BUTTON_RIGHT, mouseState, xpos, ypos));
    
}

static void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
    TwEventMouseWheelGLFW((int)yoffset);
}

static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    TwEventKeyGLFW(key, action);
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
        
        int width, height = 0;
        
        glfwGetWindowSize(window, &width, &height);
        gs::EventManager::GetInstance().Dispatch(gs::EventType::ET_KEY_PRESSED, gs::KeyEventArgs(key, width, height));
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
    gs::EventManager::GetInstance().Dispatch(gs::EventType::ET_WINDOW_RESIZED, gs::ResizeEventArgs(width, height));
    
}
