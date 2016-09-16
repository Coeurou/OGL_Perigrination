//
//  CameraUtils.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "CameraUtils.hpp"
#include "OGLUtility.hpp"
#include <iostream>

CameraUtils::CameraUtils(gs::Camera* cam) : camera(cam)
{ }

CameraUtils::~CameraUtils()
{ }

glm::vec4 ComputePlaneFromPoints(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
    glm::vec3 e1 = v2-v1;
    glm::vec3 e2 = v3-v1;
    glm::vec3 normal = glm::normalize(glm::cross(e1,e2));
    float dist = -glm::dot(normal, v2);
    
    return glm::vec4(normal, dist);
}

std::array<glm::vec4, 6> CameraUtils::GetFrustumPlanes()
{
    glm::vec3 cN = camera->GetPosition() + camera->GetForwardDirection() * camera->GetNear();
    glm::vec3 cF = camera->GetPosition() + camera->GetForwardDirection() * camera->GetFar();
    float Hnear = 2.0f * tanf(camera->GetFOV() * 0.5f) * camera->GetNear();
    float Wnear = Hnear * camera->GetAspectRatio();
    float hHnear = Hnear*0.5f;
    float hWnear = Wnear*0.5f;
    
    auto leftPlanePt = glm::normalize((cN - camera->GetRightDirection() * hWnear) - camera->GetPosition());
    auto leftPlaneNormal = glm::cross(leftPlanePt, camera->GetUpDirection());
    auto rightPlanePt = glm::normalize((cN + camera->GetRightDirection() * hWnear) - camera->GetPosition());
    auto rightPlaneNormal = glm::cross(camera->GetUpDirection(), rightPlanePt);
    
    auto topPlanePt = glm::normalize((cN + camera->GetUpDirection() * hHnear) - camera->GetPosition());
    auto topPlaneNormal = glm::cross(topPlanePt, camera->GetRightDirection());
    auto bottomPlanePt = glm::normalize((cN - camera->GetUpDirection() * hHnear) - camera->GetPosition());
    auto bottomPlaneNormal = glm::cross(camera->GetRightDirection(), bottomPlanePt);
    
    std::array<glm::vec4, 6> planes;
    planes[0] = glm::vec4(topPlaneNormal, -glm::dot(topPlaneNormal, camera->GetPosition())); // Top plane
    planes[1] = glm::vec4(bottomPlaneNormal, -glm::dot(bottomPlaneNormal, camera->GetPosition())); // Bottom plane
    planes[2] = glm::vec4(leftPlaneNormal, -glm::dot(leftPlaneNormal, camera->GetPosition())); // Left plane
    planes[3] = glm::vec4(rightPlaneNormal, -glm::dot(rightPlaneNormal, camera->GetPosition()));// Right plane
    
    planes[4] = glm::vec4(camera->GetForwardDirection(), -glm::dot(camera->GetForwardDirection(), cN)); // Near plane
    planes[5] = glm::vec4(-camera->GetForwardDirection(), -glm::dot(-camera->GetForwardDirection(), cF)); // Far plane
    return planes;
}
