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
    
    float rX = 0.0f;
    float rY = 0.0f;
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float oldX = 0.0f;
    float oldY = 0.0f;
    
    std::vector<glm::vec2> mouseHistory(10, glm::vec2(0.0f));
    const size_t MOUSE_HISTORY_BUFFER_SIZE = 10;
    const float MOUSE_FILTER_WEIGHT = 0.75f;
    
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
        view = glm::lookAt(position, target, up);
    }
    
    void Camera::Move(const glm::vec3& dir)
    {
        position += dir;
        target += dir;
        Update();
    }
    
    void Camera::Rotate(const glm::vec3& rot)
    {
        glm::mat4 r = glm::mat4(1.0f);
        r = glm::rotate(r, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
        r = glm::rotate(r, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
        r = glm::rotate(r, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
        
        up = r * glm::vec4(up, 0);
        forward = r * glm::vec4(forward, 0);
        right = glm::cross(up, forward);
        
        target = position + forward;
        
        Update();
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
    
    void filterMouseMoves()
    {
        for (int i = MOUSE_HISTORY_BUFFER_SIZE - 1; i > 0; --i) {
            mouseHistory[i] = mouseHistory[i - 1];
        }
        mouseHistory[0] = glm::vec2(rX, rY);
        float averageX = 0.0f,  averageY = 0.0f, averageTotal = 0.0f,
        currentWeight = 1.0f;
        for (int i = 0; i < MOUSE_HISTORY_BUFFER_SIZE; ++i) {
            glm::vec2 tmp = mouseHistory[i];
            averageX += tmp.x * currentWeight;
            averageY += tmp.y * currentWeight;
            averageTotal += currentWeight;
            currentWeight *= MOUSE_FILTER_WEIGHT;
        }
        mouseX = averageX / averageTotal;
        mouseY = averageY / averageTotal;
    }
    
    void Camera::OnMouseMoved(const EventArgs& args)
    {
        const auto& mouseEvent = static_cast<const MouseEventArgs&>(args);
        
        if (mouseEvent.state == 0) {
            oldX = 0;
            oldY = 0;
            return;
        }
        
        if (oldX == 0 && oldY == 0) {
            oldX = mouseEvent.posX;
            oldY = mouseEvent.posY;
        }
        
        rX += (mouseEvent.posX - oldX) * 0.2f;
        rY += (mouseEvent.posY - oldY) * 0.2f;
            
        filterMouseMoves();
            
        oldX = mouseEvent.posX;
        oldY = mouseEvent.posY;
            
        Rotate(glm::vec3(glm::radians(mouseY * speed), glm::radians(mouseX * speed), 0.0f));
    }
    
    void Camera::OnWindowResized(const EventArgs& args)
    {
        const auto& resizeEvent = static_cast<const ResizeEventArgs&>(args);
        SetupProjection(fov, resizeEvent.width/(float)resizeEvent.height);
    }
}