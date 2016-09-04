//
//  Camera.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 14/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

namespace gs
{
    class EventArgs;
    
    class Camera
    {
		double lastTime = 0;
		double deltaTime = 0;
		glm::ivec2 mousePos;
		float horizontalAngle = 0.0f;	// angle on Y axis (gaze left to right)
		float verticalAngle = 0.0f;		// angle on X axis (gaze uup to down)

    public:
        Camera();
        virtual ~Camera();
        
        void Update();
        void Move(const glm::vec3& dir);
        void Rotate(const glm::vec2& rot);
        void SetupProjection(float fovy, float aspectRatio, float near = 0.1f, float far = 100.0f);

        void OnKeyPressed(const EventArgs& args);
        void OnMouseMoved(const EventArgs& args);
        void OnWindowResized(const EventArgs& args);
        
        glm::mat4 GetProjectionMatrix() const { return projection; }
        glm::mat4 GetViewMatrix() const { return view; }
        glm::mat4 GetViewProjectionMatrix() const { return projection * view; }
		glm::vec3 GetPosition() const { return position; }
		glm::vec3 GetForwardVector() const { return forward; }
		glm::vec3 GetUpVector() const { return up; }
        void SetSpeed(float velocity) { speed = velocity; }
        void SetAngularSpeed(float velocity) { angularSpeed = velocity; }
        void SetPosition(const glm::vec3& newPos) { position = newPos; Update(); }
        void SetTarget(const glm::vec3& newTarget) { target = newTarget; Update(); }
        void SetUpVector(const glm::vec3& dir) { up = dir; Update(); }
        
    protected:
        glm::vec3 position;        
        glm::vec3 target;
        
        float fov;
        float nearDistance;
        float farDistance;
        float speed;
        float angularSpeed;
        
        glm::mat4 view;
        glm::mat4 projection;
        
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 forward;
    };
}
/* Camera_hpp */
