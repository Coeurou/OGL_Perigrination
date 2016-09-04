//
//  Material.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "OGLResource.hpp"
#include "Texture.hpp"

namespace gs
{
    struct Material : public OGLResource
    {
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec4 emissiveColor;
		float shininess;
    };
}
/* Material_hpp */
