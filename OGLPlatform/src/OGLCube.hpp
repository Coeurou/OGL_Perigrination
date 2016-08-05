#pragma once

#include "OGLObjResizable.hpp"
#include <array>

enum SIZE { CUBE = 36 };

class OGLCube : public OGLObjResizable // A templater voir OGLQuad
{
public:
	OGLCube();
	~OGLCube();

    bool InitVertices(glm::vec3 origin);
    bool InitVertices(std::array<glm::vec3, 8> data);
};

