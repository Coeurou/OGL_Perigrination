//
//  Vertex.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

namespace gs
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texCoords;
		glm::vec3 normal;
	};

	struct ColoredVertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};
}
/* OGLVertex_hpp */
