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
    class Material : public OGLResource
    {
    public:
        Material();
        ~Material();
        
        bool AddTexture(const std::string& imgFilename, LIGHT_CONTRIBUTION contribution);

		std::vector<std::shared_ptr<Texture>> GetTextures() const { return textures; }

		glm::vec4 GetAmbientColor() const { return ambientColor; }
		void SetAmbientColor(glm::vec4 color) { ambientColor = color; }

		glm::vec4 GetDiffuseColor() const { return diffuseColor; }
		void SetDiffuseColor(glm::vec4 color) { diffuseColor = color; }

		glm::vec4 GetSpecularColor() const { return specularColor; }
		void SetSpecularColor(glm::vec4 color) { specularColor = color; }

		glm::vec4 GetEmissiveColor() const { return emissiveColor; }
		void SetEmissiveColor(glm::vec4 color) { emissiveColor = color; }

		float GetShininess() const { return shininess; }
		void SetShininess(float index) { shininess = index; }

    private:
		std::vector<std::shared_ptr<Texture>> textures;
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec4 emissiveColor;
		float shininess;
    };
}
/* Material_hpp */
