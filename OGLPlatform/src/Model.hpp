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
#include <assimp/scene.h>
#include "OGLRenderableObject.hpp"
#include "Material.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

struct aiScene;
struct aiMesh;
struct aiMaterial;
enum class LIGHT_CONTRIBUTION;

namespace gs
{
	class Mesh;
	struct Material;

	class Model : public OGLRenderableObject
	{
	public:
		Model();
		~Model();

		GLuint GetVAO() const { return vao.get(); }
		const std::vector<Mesh>& GetSubMeshes() { return subMeshes; };

		bool Load(const std::string& meshFilename) override;
		void Render(Program* program) override;

	private:	
		class AssimpMaterial
		{
		public:
			std::shared_ptr<Material> material;
			std::vector<std::shared_ptr<Texture>> textures;			

			glm::vec4 GetAmbientColor() const { return material->ambientColor; }
			void SetAmbientColor(glm::vec4 color) { material->ambientColor = color; }

			glm::vec4 GetDiffuseColor() const { return material->diffuseColor; }
			void SetDiffuseColor(glm::vec4 color) { material->diffuseColor = color; }

			glm::vec4 GetSpecularColor() const { return material->specularColor; }
			void SetSpecularColor(glm::vec4 color) { material->specularColor = color; }

			glm::vec4 GetEmissiveColor() const { return material->emissiveColor; }
			void SetEmissiveColor(glm::vec4 color) { material->emissiveColor = color; }

			float GetShininess() const { return material->shininess; }
			void SetShininess(float index) { material->shininess = index; }
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
