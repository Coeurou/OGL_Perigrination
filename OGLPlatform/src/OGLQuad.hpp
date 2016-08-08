#pragma once

#include "OGLObjResizable.hpp"
#include <array>

namespace gs
{
    struct Vertex;
}

class OGLQuad : public OGLObjResizable // TODO a templater par OGLShape<size_t nb vertices>
{
public:
	OGLQuad();
	~OGLQuad();

    bool InitVertices(glm::vec3 origin);
    bool InitVertices(std::array<gs::Vertex, 4> data);
};