//
//  OGLMultipleLights.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 06/09/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "Stage.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"

class OGLMultipleLights : public gs::Stage
{
    gs::DirectionnalLight dirLight;
    gs::SpotLight spotLight;
    std::vector<gs::PointLight> ptLights;
    gs::Camera camera;
    std::vector<glm::vec3> cubePositions;
    
public:
    OGLMultipleLights();
    ~OGLMultipleLights();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI() override;
    void InitCubePosition();
    void InitLight();
    bool AddProgram(const std::string& vsFilename, const std::string& fsFilename);
    bool AddMatricesUniform(gs::Program* program);
    bool AddLightUniform(gs::Program* program);
    void SendLightUniform(const gs::Program* program);
    void SendMatricesUniform(const gs::Program* program, const glm::mat4& model);
};
/* OGLMultipleLights_hpp */
