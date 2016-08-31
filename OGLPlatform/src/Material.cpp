//
//  Texture.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "Material.hpp"

namespace gs
{
	Material::Material()
    {
	}
    
	Material::~Material()
    {
	}
    
    bool Material::AddTexture(const std::string& imgFilename, LIGHT_CONTRIBUTION contribution)
    {
		auto texture = std::make_shared<Texture>(IMAGE_TYPE::GLI);
		bool res = texture->LoadTexture(imgFilename);
		texture->SetContribution(contribution);
		textures.push_back(texture);

        return res;
    }
}