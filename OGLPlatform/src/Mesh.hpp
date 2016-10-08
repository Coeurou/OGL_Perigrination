//
//  Mesh.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <vector>
#include "OGLRenderableObject.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "Material.hpp"

namespace gs
{
	class Program;

    class Mesh : public OGLRenderableObject
	{
	public:
		Mesh();
		~Mesh();

        bool Load(const std::string& meshFilename) override { return false; };
		void InitGL(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
		void SetMaterial(const Material& material, const std::vector<std::shared_ptr<Texture>>& textures) { this->material = material; this->textures = textures; }
		void Render() override;
		void Render(int nbInstances) override;

		Texture* GetTexture(int index) const { return textures[index].get(); }

		int GetMaterialIndex() const { return materialIndex; }
		void SetMaterialIndex(int index) { materialIndex = index; }

		int GetNbIndices() const { return nbIndices; }
		void SetNbIndices(int size) { nbIndices = size; }

		glm::mat4 GetModelMatrix() const { return modelMatrix; }
		VertexBuffer GetVBO() const { return vbo; }
		VertexBuffer GetIBO() const { return ibo; }

	private:
		void PrepareDraw();

		VertexBuffer vbo;
		VertexBuffer ibo;
		int materialIndex;
		int nbIndices;
		glm::mat4 modelMatrix;
					
		Material material;
		std::vector<std::shared_ptr<Texture>> textures;

	};
}
/* Mesh.hpp */
