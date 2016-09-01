//
//  Mesh.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 30/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>
#include "OGLRenderableObject.hpp"
#include "Material.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
enum class LIGHT_CONTRIBUTION;

namespace gs
{
	class Mesh;
	class Material;

	class Model : public OGLRenderableObject
	{
	public:
		Model();
		~Model();

		GLuint GetVAO() const { return vao.get(); }

		bool Load(const std::string& meshFilename) override;
		void Render(Program* program) override;

	private:	
		class AssimpMaterial
		{
		public:
			std::shared_ptr<Material> material;
			std::vector<std::shared_ptr<Texture>> textures;

			glm::vec4 GetAmbientColor() const { return material->GetAmbientColor(); }
			void SetAmbientColor(glm::vec4 color) { material->SetAmbientColor(color); }

			glm::vec4 GetDiffuseColor() const { return material->GetDiffuseColor(); }
			void SetDiffuseColor(glm::vec4 color) { material->SetDiffuseColor(color); }

			glm::vec4 GetSpecularColor() const { return material->GetSpecularColor(); }
			void SetSpecularColor(glm::vec4 color) { material->SetSpecularColor(color); }

			glm::vec4 GetEmissiveColor() const { return material->GetEmissiveColor(); }
			void SetEmissiveColor(glm::vec4 color) { material->SetEmissiveColor(color); }

			float GetShininess() const { return material->GetShininess(); }
			void SetShininess(float index) { material->SetShininess(index); }
		};

		bool InitFromScene(const aiScene* scene);
		bool InitMesh(unsigned int index, const aiMesh* mesh, AssimpMaterial* assimpMaterial);
		bool InitMaterials(const aiScene* scene);
		bool InitMaterialByType(const aiMaterial* material, const aiTextureType texType, unsigned int startIndex);
		bool AddTexture(int materialIndex, const std::string& imgFilename, LIGHT_CONTRIBUTION contribution);

		std::vector<Mesh> subMeshes;
		std::vector<std::shared_ptr<AssimpMaterial>> materials;
		VertexArray vao;
	};
}
/* OGLTexture_hpp */
