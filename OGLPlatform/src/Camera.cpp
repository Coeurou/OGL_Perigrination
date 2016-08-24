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
    Camera::Camera() : speed(0.1f), nearDistance(0.1f), farDistance(100.0f), fov(45.0f),
                       up(glm::vec3(0.0f, 1.0f, 0.0f)), right(glm::vec3(1.0f, 0.0f, 0.0f)),
                       target(glm::vec3(0.0f, 0.0f, 1.0f)), forward(glm::vec3(0.0f, 0.0f, 1.0f))
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
    
    void Camera::Rotate(const glm::vec3& rot)
    {   
        forward = glm::vec3(cos(rot.x) * sin(rot.y), sin(rot.x), cos(rot.x) * cos(rot.y));
		glm::normalize(forward);
        right = glm::vec3(sin(rot.y - glm::half_pi<float>()), 0, cos(rot.y - glm::half_pi<float>()));
		glm::normalize(right);
		up = glm::cross(right, forward);

        target = position + forward;
    }
    
    void Camera::SetupProjection(float fovy, float aspectRatio, float near, float far)
    {
        fov = fovy;
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
    
    void Camera::OnMouseMoved(const EventArgs& args)
    {
        const auto& mouseEvent = static_cast<const MouseEventArgs&>(args);
        
		int deltaX = mouseEvent.posX - mousePos.x;
		int deltaY = mouseEvent.posY - mousePos.y;

		mousePos = glm::ivec2(mouseEvent.posX, mouseEvent.posY);

		// On mouse button release
        if (mouseEvent.state == 0) {
            return;
        }

		horizontalAngle += deltaX;
		verticalAngle += deltaY;
            
        Rotate(glm::vec3(glm::radians(-verticalAngle * (float)deltaTime * speed), glm::radians(-horizontalAngle * (float)deltaTime * speed), 0.0f));
    }
    
    void Camera::OnWindowResized(const EventArgs& args)
    {
        const auto& resizeEvent = static_cast<const ResizeEventArgs&>(args);
        SetupProjection(fov, resizeEvent.width/(float)resizeEvent.height);
    }
}