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
    Camera::Camera() : velocity(0.1f), angularSpeed(0.5f), nearDistance(0.1f), farDistance(100.0f),
                       fov(45.0f), up(glm::vec3(0.0f, 1.0f, 0.0f)),
                       right(glm::vec3(1.0f, 0.0f, 0.0f)), target(glm::vec3(0.0f, 0.0f, 0.0f)),
                       forward(glm::vec3(0.0f, 0.0f, -1.0f)), accumPitchRadians(0.0f),
					   acceleration(8.0f), currentVelocity(0.0f), direction(0.0f)
    {
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_KEY, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_MOVED, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_WINDOW_RESIZED, this);
    }
    
    Camera::~Camera()
    {
		gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_KEY, this);
		gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_MOVED, this);
		gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_WINDOW_RESIZED, this);
    }

	void Camera::OnEvent(Event e)
	{
		switch (e.GetEventType())
		{
		case EventType::ET_KEY:
			if (e.args.keyState >= (int)ButtonState::BUTTON_PRESSED) {
				OnKeyPressed(e);
			}
			else {
				OnKeyReleased(e);
			}
			break;
		case EventType::ET_MOUSE_MOVED:
			OnMouseMoved(e);
			break;
		case EventType::ET_WINDOW_RESIZED:
			OnWindowResized(e);
			break;
		default:
			break;
		}
	}

	void Camera::UpdateVelocity()
	{
		currentVelocity += direction * acceleration * (float)deltaTime;
		// X axis
		if (direction.x != 0.0f) {
			if (currentVelocity.x > velocity) {
				currentVelocity.x = velocity;
			}
			else if (currentVelocity.x < -velocity) {
				currentVelocity.x = -velocity;
			}
		}
		else
		{
			if (currentVelocity.x > 0.0f) {
				if (currentVelocity.x -= acceleration.x * deltaTime < 0.0f) {
					currentVelocity.x = 0.0f;
				}
			}
			else if (currentVelocity.x < 0.0f) {
				if (currentVelocity.x += acceleration.x * deltaTime > 0.0f) {
					currentVelocity.x = 0.0f;
				}
			}
		}

		// Y axis
		if (direction.y != 0.0f) {
			if (currentVelocity.y > velocity) {
				currentVelocity.y = velocity;
			}
			else if (currentVelocity.y < -velocity) {
				currentVelocity.y = -velocity;
			}
		}
		else
		{
			if (currentVelocity.y > 0.0f) {
				if (currentVelocity.y -= acceleration.y * deltaTime < 0.0f) {
					currentVelocity.y = 0.0f;
				}
			}
			else if (currentVelocity.y < 0.0f) {
				if (currentVelocity.y += acceleration.y * deltaTime > 0.0f) {
					currentVelocity.y = 0.0f;
				}
			}
		}

		// Z axis
		if (direction.z != 0.0f) {
			if (currentVelocity.z > velocity) {
				currentVelocity.z = velocity;
			}
			else if (currentVelocity.z < -velocity) {
				currentVelocity.z = -velocity;
			}
		}
		else
		{
			if (currentVelocity.z > 0.0f) {
				if (currentVelocity.z -= acceleration.z * deltaTime < 0.0f) {
					currentVelocity.z = 0.0f;
				}
			}
			else if (currentVelocity.z < 0.0f) {
				if (currentVelocity.z += acceleration.z * deltaTime > 0.0f) {
					currentVelocity.z = 0.0f;
				}
			}
		}
	}

	void Camera::UpdatePosition()
	{		
		if (glm::length(currentVelocity) * glm::length(currentVelocity) != 0.0f)
		{
			glm::vec3 displacement = currentVelocity * (float)deltaTime + acceleration * (float)(deltaTime * deltaTime);
			position += displacement;
			target = position + forward;
			UpdateVelocity();
		}
	}

    void Camera::Update()
    {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		UpdatePosition();
        view = glm::lookAt(position, target, up);
    }
    
    void Camera::Move(const glm::vec3& dir)
    {
		direction = dir;
		currentVelocity = dir * velocity;
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
    
	void Camera::OnKeyPressed(const Event& e)
	{
		switch (e.args.key)
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
			Move(forward);
			break;
		case GLFW_KEY_S:
			Move(-forward);
			break;
		case GLFW_KEY_D:
			Move(right);
			break;
		case GLFW_KEY_A:
			Move(-right);
			break;
		case GLFW_KEY_Q:
			Move(up);
			break;
		case GLFW_KEY_E:
			Move(-up);
			break;
		default:
			break;
		}
		if (e.args.key >= GLFW_KEY_0 && e.args.key <= GLFW_KEY_8)
		{
			projection = glm::perspective(fov, windowWidth / (float)windowHeight, nearDistance, farDistance);
		}
	}

    void Camera::OnKeyReleased(const Event& e)
    {
        switch (e.args.key)
        {            
            case GLFW_KEY_W:
            case GLFW_KEY_S:
            case GLFW_KEY_D:
            case GLFW_KEY_A:
            case GLFW_KEY_Q:
            case GLFW_KEY_E:
				direction = glm::vec3(0);
                break;
            default:
                break;
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
    
    void Camera::OnMouseMoved(const Event& e)
    {
		// If mouse button release or button is not right
		if (e.args.mouseButton != (int)MouseButton::MB_RIGHT || e.args.mouseButtonState == (int)ButtonState::BUTTON_RELEASED) { return; }

        glm::vec2 delta { e.args.mousePosY - mousePos.y, e.args.mousePosX - mousePos.x };

		mousePos = glm::ivec2(e.args.mousePosX, e.args.mousePosY);
        delta = FilterMousePos(delta);
        if (glm::length(delta) > maxMouseMove) {
            return;
        }
        delta *= deltaTime * angularSpeed;
        
		accumPitchRadians += delta.x;
		if (accumPitchRadians > glm::half_pi<float>())
		{
			accumPitchRadians = glm::half_pi<float>();
			delta.x = 0;
		}
		else if (accumPitchRadians < -glm::half_pi<float>())
		{
			accumPitchRadians = -glm::half_pi<float>();
			delta.x = 0;
		}
		Rotate(delta);	
    }
    
    void Camera::OnWindowResized(const Event& e)
    {
		if (e.args.width > 0) {
			SetupProjection(fov, e.args.width / (float)e.args.height);
			SetWindowSize(e.args.width, e.args.height);
		}
    }
}
