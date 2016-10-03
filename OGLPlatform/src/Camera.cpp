//
//  Camera.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 14/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Camera.hpp"
#include "EventManager.hpp"
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


namespace gs
{ 
    Camera::Camera() : speed(0.1f), angularSpeed(0.5f), nearDistance(0.1f), farDistance(100.0f),
                       fov(45.0f), up(glm::vec3(0.0f, 1.0f, 0.0f)),
                       right(glm::vec3(1.0f, 0.0f, 0.0f)), target(glm::vec3(0.0f, 0.0f, 0.0f)),
                       forward(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        EventFun keyEventFun(0, [=](const EventArgs& args) {
            this->OnKeyPressed(args);
        });
        EventManager::GetInstance().Subscribe(std::make_pair(EventType::ET_KEY_PRESSED, keyEventFun));
        
        EventFun mouseEventFun(0, [=](const EventArgs& args) {
            this->OnMouseMoved(args);
        });
        EventManager::GetInstance().Subscribe(std::make_pair(EventType::ET_MOUSE_MOVED, mouseEventFun));
        
        EventFun resizeEventFun(0, [=](const EventArgs& args) {
            this->OnWindowResized(args);
        });
        EventManager::GetInstance().Subscribe(std::make_pair(EventType::ET_WINDOW_RESIZED, resizeEventFun));
    }
    
    Camera::~Camera()
    {
        EventFun keyEventFun(0, [=](const EventArgs& args) {
            this->OnKeyPressed(args);
        });
        EventManager::GetInstance().Unsubscribe(std::make_pair(EventType::ET_KEY_PRESSED, keyEventFun));
        
        EventFun mouseEventFun(0, [=](const EventArgs& args) {
            this->OnMouseMoved(args);
        });
        EventManager::GetInstance().Unsubscribe(std::make_pair(EventType::ET_MOUSE_MOVED, mouseEventFun));
        
        EventFun resizeEventFun(0, [=](const EventArgs& args) {
            this->OnWindowResized(args);
        });
        EventManager::GetInstance().Unsubscribe(std::make_pair(EventType::ET_WINDOW_RESIZED, resizeEventFun));
    }
    
    void Camera::Update()
    {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
        view = glm::lookAt(position, target, up);
    }
    
    void Camera::Move(const glm::vec3& dir)
    {
        position += dir * (float)deltaTime;
        target = position + forward;
    }
    
    void Camera::Rotate(const glm::vec2& rot)
    {
        forward = glm::rotate(glm::mat4(1.0f), -rot.y, up) * glm::rotate(glm::mat4(1.0f), -rot.x, right) * glm::vec4(forward, 0);
		glm::normalize(forward);
		right = glm::cross(forward, up);
		glm::normalize(right);

        target = position + forward;
    }
    
    void Camera::SetupProjection(float fovy, float aspectRatio, float near, float far)
    {
        fov = fovy;
        this->aspectRatio = aspectRatio;
        nearDistance = near;
        farDistance = far;
        projection = glm::perspective(fovy, aspectRatio, near, far);
    }
    
    void Camera::OnKeyPressed(const EventArgs& args)
    {
        const auto& keyEvent = static_cast<const KeyEventArgs&>(args);
        
        switch (keyEvent.key)
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
                nearDistance -= 0.1f;
                break;
            case GLFW_KEY_7:
                farDistance += 1.0f;
                break;
            case GLFW_KEY_8:
                farDistance -= 1.0f;
                break;
            case GLFW_KEY_W:
                Move(forward * speed);
                break;
            case GLFW_KEY_S:
                Move(-forward * speed);
                break;
            case GLFW_KEY_D:
                Move(right * speed);
                break;
            case GLFW_KEY_A:
                Move(-right * speed);
                break;
            case GLFW_KEY_Q:
                Move(up * speed);
                break;
            case GLFW_KEY_E:
                Move(-up * speed);
                break;
            default:
                break;
        }
        if (keyEvent.key >= GLFW_KEY_0 && keyEvent.key <= GLFW_KEY_8)
        {
            projection = glm::perspective(fov, keyEvent.width / (float)keyEvent.height, nearDistance, farDistance);
        }
    }
    
    glm::vec2 Camera::FilterMousePos(const glm::vec2& mousePos)
    {
        for (size_t i = HISTORY_BUFFER_SIZE-1; i > 0; i--) {
            mouseHistory[i] = mouseHistory[i-1];
        }
        mouseHistory[0] = mousePos;
        
        glm::vec2 smoothPos;
        float weight = 1.0f;
        float averageSum = 0.0f;
        
        for (auto pos : mouseHistory) {
            averageSum += weight;
            smoothPos += pos * weight;
            weight *= WEIGHT_MODIFIER;
        }
        return smoothPos / averageSum;
    }
    
    void Camera::OnMouseMoved(const EventArgs& args)
    {
        const auto& mouseEvent = static_cast<const MouseEventArgs&>(args);
		// If mouse button release or button is not right
		if (mouseEvent.button != 1 || mouseEvent.state == 0) { return; }

        glm::vec2 delta { mouseEvent.posY - mousePos.y, mouseEvent.posX - mousePos.x };

		mousePos = glm::ivec2(mouseEvent.posX, mouseEvent.posY);
        delta = FilterMousePos(delta);
        if (glm::length(delta) > maxMouseMove) {
            return;
        }
        delta *= deltaTime * angularSpeed;
            
        Rotate(delta);
    }
    
    void Camera::OnWindowResized(const EventArgs& args)
    {
        const auto& resizeEvent = static_cast<const ResizeEventArgs&>(args);
		if (resizeEvent.width > 0) {
			SetupProjection(fov, resizeEvent.width / (float)resizeEvent.height);
		}
    }
}
