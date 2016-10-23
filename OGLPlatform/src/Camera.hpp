//
//  Camera.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 14/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "EventListener.hpp"
#include <glm/glm.hpp>
#include <array>

namespace gs
{
    class Event;
	enum { HISTORY_BUFFER_SIZE = 10 };

    class Camera : public gs::IEventListener
    {
        const float maxMouseMove = 40.0f;
        const float WEIGHT_MODIFIER = 0.3f;
        
        std::array<glm::vec2, HISTORY_BUFFER_SIZE> mouseHistory;
        std::array<glm::vec3, HISTORY_BUFFER_SIZE> keyHistory;
		double lastTime = 0;
		double deltaTime = 0;
		glm::vec2 mousePos;
		float horizontalAngle = 0.0f;	// angle on Y axis (gaze left to right)
		float verticalAngle = 0.0f;		// angle on X axis (gaze uup to down)

    public:
        Camera();
        virtual ~Camera();
        
		void OnEvent(Event e) override;

        void Update();
        void Move(const glm::vec3& dir);
        void Rotate(const glm::vec2& rot);
        void SetupProjection(float fovy, float aspectRatio, float near = 0.1f, float far = 100.0f);

        void OnKeyPressed(const Event& e);
		void OnKeyReleased(const Event& e);
        void OnMouseMoved(const Event& e);
        void OnWindowResized(const Event& e);
        
        glm::mat4 GetProjectionMatrix() const { return projection; }
        glm::mat4 GetViewMatrix() const { return view; }
        glm::mat4 GetViewProjectionMatrix() const { return projection * view; }
        
		glm::vec3 GetPosition() const { return position; }
		glm::vec3 GetForwardDirection() const { return forward; }
		glm::vec3 GetUpDirection() const { return up; }
        glm::vec3 GetRightDirection() const { return right; }
        glm::vec3 GetTarget() const { return target; }
        
        float GetFOV() const { return fov; }
        float GetAspectRatio() const { return aspectRatio; }
        float GetNear() const { return nearDistance; }
        float GetFar() const { return farDistance; }
        float GetSpeed() const { return velocity; }
        float GetAngularSpeed() const { return angularSpeed; }
        
        void SetSpeed(float velocity) { this->velocity = velocity; }
        void SetAngularSpeed(float velocity) { angularSpeed = velocity; }
        void SetPosition(const glm::vec3& newPos) { position = newPos; }
        void SetTarget(const glm::vec3& newTarget) { target = newTarget; }
		void SetUpVector(const glm::vec3& dir) { up = dir; }
		void SetWindowSize(int w, int h) { windowWidth = w; windowHeight = h; }
        
	private:
        glm::vec3 position;        
        glm::vec3 target;
        
        float fov;
        float nearDistance;
        float farDistance;
		float velocity;
        float angularSpeed;
		float aspectRatio;
		float accumPitchRadians;
		int windowWidth;
		int windowHeight;

        glm::mat4 view;
        glm::mat4 projection;
        
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 forward;

		glm::vec3 currentVelocity;
		glm::vec3 acceleration;
		glm::vec3 direction;

		glm::vec2 FilterMousePos(const glm::vec2& mousePos);
		void UpdatePosition();
		void UpdateVelocity();
    };
}
/* Camera_hpp */
