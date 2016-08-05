#pragma once

#include "OGLObjResizable.hpp"
#include <array>

class OGLQuad : public OGLObjResizable // TODO a templater par OGLShape<size_t nb vertices>
{
public:
	OGLQuad();
	~OGLQuad();

    bool InitVertices(glm::vec3 origin);
    bool InitVertices(std::array<OGLVertex, 4> data);
};