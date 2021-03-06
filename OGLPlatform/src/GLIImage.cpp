//
//  GLIImage.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 31/07/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "GLIImage.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <gli/gli.hpp>

GLIImage::GLIImage()
{}

GLIImage::~GLIImage()
{}

bool GLIImage::LoadImageFile(const std::string& imgPath)
{
    textureID = 0;
    gli::texture texture = gli::load(imgPath);
	if (texture.empty()) {
		std::cerr << "Cannot load image file:" << imgPath << std::endl;
		return false;
	}

    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format const texFormat = GL.translate(texture.format(), texture.swizzles());
    target = (GLenum)GL.translate(texture.target());
    
    glGenTextures(1, &textureID);
    glBindTexture(target, textureID);
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, texFormat.Swizzles[0]);
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, texFormat.Swizzles[1]);
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, texFormat.Swizzles[2]);
    glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, texFormat.Swizzles[3]);
    
    glm::tvec3<GLsizei> const extent(texture.extent());
    GLsizei const faceTotal = static_cast<GLsizei>(texture.layers() * texture.faces());
	nbFaces = (int)faceTotal;

    switch(texture.target())
    {
        case gli::TARGET_1D:
            glTexStorage1D(target, static_cast<GLint>(texture.levels()), texFormat.Internal,
                                                      extent.x);
            break;
        case gli::TARGET_1D_ARRAY:
        case gli::TARGET_2D:
        case gli::TARGET_CUBE:
            glTexStorage2D(target, static_cast<GLint>(texture.levels()), texFormat.Internal,
                                                      extent.x, texture.target() == gli::TARGET_2D ? extent.y : faceTotal);
            break;
        case gli::TARGET_2D_ARRAY:
        case gli::TARGET_3D:
        case gli::TARGET_CUBE_ARRAY:
            glTexStorage3D(target, static_cast<GLint>(texture.levels()), texFormat.Internal,
                                                      extent.x, extent.y,
                                                      texture.target() == gli::TARGET_3D ? extent.z : faceTotal);
            break;
        default:
            textureID = 0;
            std::cerr << "Texture target not found in gli::texture::target.";
            break;
    }
    for(std::size_t layer = 0; layer < texture.layers(); ++layer) {
        for(std::size_t face = 0; face < texture.faces(); ++face) {
            for(std::size_t level = 0; level < texture.levels(); ++level) {
                GLsizei const layerGL = static_cast<GLsizei>(layer);
                glm::tvec3<GLsizei> extent(texture.extent(level));
                target = gli::is_target_cube(texture.target()) ?
                static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face) :
                target;
                
                switch(texture.target())
                {
                    case gli::TARGET_1D:
                        if(gli::is_compressed(texture.format()))
                            glCompressedTexSubImage1D(target,
                                                      static_cast<GLint>(level), 0, extent.x,
                                                      texFormat.Internal,
                                                      static_cast<GLsizei>(texture.size(level)),
                                                      texture.data(layer, face, level));
                        else
                            glTexSubImage1D(target, static_cast<GLint>(level), 0, extent.x,
                                            texFormat.External, texFormat.Type,
                                            texture.data(layer, face, level));
                        break;
                    case gli::TARGET_1D_ARRAY:
                    case gli::TARGET_2D:
                    case gli::TARGET_CUBE:
                        if(gli::is_compressed(texture.format()))
                            glCompressedTexSubImage2D(target, static_cast<GLint>(level),
                                                      0, 0,
                                                      extent.x,
                                                      texture.target() == gli::TARGET_1D_ARRAY ? layerGL : extent.y,
                                                      texFormat.Internal,
                                                      static_cast<GLsizei>(texture.size(level)),
                                                      texture.data(layer, face, level));
                        else
                            glTexSubImage2D(target, static_cast<GLint>(level),
                                            0, 0,
                                            extent.x,
                                            texture.target() == gli::TARGET_1D_ARRAY ? layerGL : extent.y,
                                            texFormat.External, texFormat.Type,
                                            texture.data(layer, face, level));
                        break;
                    case gli::TARGET_2D_ARRAY:
                    case gli::TARGET_3D:
                    case gli::TARGET_CUBE_ARRAY:
                        if(gli::is_compressed(texture.format()))
                            glCompressedTexSubImage3D(target, static_cast<GLint>(level),
                                                      0, 0, 0,
                                                      extent.x, extent.y,
                                                      texture.target() == gli::TARGET_3D ? extent.z : layerGL,
                                                      texFormat.Internal,
                                                      static_cast<GLsizei>(texture.size(level)),
                                                      texture.data(layer, face, level));
                        else
                            glTexSubImage3D(target, static_cast<GLint>(level),
                                            0, 0, 0,
                                            extent.x, extent.y,
                                            texture.target() == gli::TARGET_3D ? extent.z : layerGL,
                                            texFormat.External, texFormat.Type,
                                            texture.data(layer, face, level));
                        break;
                    default:
                        textureID = 0;
                        std::cerr << "Texture target not found in gli::texture::target.";
                        break;
                }
            }
        }
    }

    return (textureID != 0);
}

bool GLIImage::LoadCubemapFile(const std::vector<std::string>& imgPaths)
{
	textureID = 0;
	glGenTextures(1, &textureID);
	target = GL_TEXTURE_CUBE_MAP;
	glBindTexture(target, textureID);
	int face = 0;

	for (auto& imgPath : imgPaths) {
		gli::texture texture = gli::load(imgPath);
		if (texture.empty()) {
			std::cerr << "Cannot load image file:" << imgPath << std::endl;
			return false;
		}

		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const texFormat = GL.translate(texture.format(), texture.swizzles());

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, texFormat.Swizzles[0]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, texFormat.Swizzles[1]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, texFormat.Swizzles[2]);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, texFormat.Swizzles[3]);

		glm::tvec3<GLsizei> const extent(texture.extent());
		GLsizei const faceTotal = static_cast<GLsizei>(texture.layers() * texture.faces());
		nbFaces = (int)faceTotal;

		if (face == 0) {
			glTexStorage2D(target, static_cast<GLint>(texture.levels()), texFormat.Internal,
				extent.x, texture.target() == gli::TARGET_2D ? extent.y : faceTotal);
		}

		target = static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);
		if (gli::is_compressed(texture.format()))
			glCompressedTexSubImage2D(target, 0,
				0, 0,
				extent.x,
				extent.y,
				texFormat.Internal,
				static_cast<GLsizei>(texture.size(0)),
				texture.data(0, 0, 0));
		else
			glTexSubImage2D(target, 0,
				0, 0,
				extent.x,
				extent.y,
				texFormat.External, texFormat.Type,
				texture.data(0, 0, 0));
		face++;
	}
    target = GL_TEXTURE_CUBE_MAP;
	return (textureID != 0);
}
