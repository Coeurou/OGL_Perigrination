//
//  OGLIndexedDraw.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 12/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include "Stage.hpp"

class OGLIndexedDraw : public gs::Stage
{
	const short HM_SIZE_X = 4;
	const short HM_SIZE_Z = 4;
	const float WORLD_SIZE_X = 40.0f;
	const float WORLD_SIZE_Z = 40.0f;
	std::vector<float> heights = std::vector<float>(HM_SIZE_X * HM_SIZE_Z);
	std::vector<glm::vec3> vertices = std::vector<glm::vec3>(HM_SIZE_X * HM_SIZE_Z);
	std::vector<GLushort> indices = std::vector<GLushort>(HM_SIZE_X * 2 * (HM_SIZE_Z - 1) + HM_SIZE_Z - 2);

	float heightDivider = 12.0f;
	GLint hDivLocation = 0;

public:
    OGLIndexedDraw();
    ~OGLIndexedDraw();
    
    bool Init(int windowWidth, int windowHeight) override;
    void Render(double time) override;
    
private:
    bool InitGUI() override;
};
/* OGLIndexedDraw_hpp */
