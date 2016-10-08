#include "Model.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "OGLApplicationConstants.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

namespace gs
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	bool Model::Load(const std::string& meshFilename)
	{
		Assimp::Importer importer;
		auto scene = importer.ReadFile(ModelsPath + "/" + meshFilename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
																		aiProcess_JoinIdenticalVertices | aiProcess_ValidateDataStructure |
																		aiProcess_SortByPType | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

		if (scene != nullptr) {
			vao.BindVAO();
			vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
			vao.AddAttribute(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			return InitFromScene(scene);
		}
		else {
			std::cerr << "Cannot load model: " << meshFilename << " Error: " << importer.GetErrorString() << std::endl;
		}
		return false;
	}

	bool Model::InitFromScene(const aiScene * scene)
	{
		bool res = true;
		children.resize(scene->mNumMeshes);
		materials.resize(scene->mNumMaterials);

		res &= InitMaterials(scene);
		for (unsigned int i = 0; i < children.size(); i++) {
			const aiMesh* mesh = scene->mMeshes[i];
			res &= InitMesh(i, mesh, materials[mesh->mMaterialIndex]);
		}
		return res;
	}

	bool Model::InitMesh(unsigned int index, const aiMesh* mesh, const AssimpMaterial& assimpMaterial)
	{
        children[index] = std::make_unique<Mesh>();
		auto gsMesh = dynamic_cast<Mesh*>(children[index].get());
		gsMesh->SetMaterialIndex(mesh->mMaterialIndex);

		std::vector<gs::Vertex> vertices;
		std::vector<GLuint> indices;

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			const aiVector3D* pPos = &(mesh->mVertices[i]);
			const aiVector3D* pNormal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &Zero3D;
			const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;

			Vertex v { glm::vec3(pPos->x, pPos->y, pPos->z),
					   glm::vec2(pTexCoord->x, pTexCoord->y),
					   glm::vec3(pNormal->x, pNormal->y, pNormal->z) };

			vertices.push_back(v);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			const aiFace& face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
		gsMesh->SetProgram(program);
		gsMesh->InitGL(vertices, indices);
		gsMesh->SetMaterial(assimpMaterial.material, assimpMaterial.textures);
		return true;
	}

	bool replace(std::string& str, const std::string& from, const std::string& to) 
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	bool Model::AddTexture(int materialIndex, const std::string& imgFilename, LIGHT_CONTRIBUTION contribution)
	{
		auto texture = std::make_shared<Texture>(IMAGE_TYPE::GLI);
		bool res = texture->LoadTexture(imgFilename);
		texture->SetContribution(contribution);
		materials[materialIndex].textures.push_back(texture);

		return res;
	}

	bool Model::InitMaterials(const aiScene * scene)
	{
		bool res = true;
		for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
			const aiMaterial* material = scene->mMaterials[i];
			if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
				res &= InitMaterialByType(material, aiTextureType_DIFFUSE, i);
			}
			if (material->GetTextureCount(aiTextureType_SPECULAR)) {
				res &= InitMaterialByType(material, aiTextureType_SPECULAR, i);
			}
		}
		return res;
	}

	bool Model::InitMaterialByType(const aiMaterial * material, const aiTextureType texType, unsigned int startIndex)
	{
		bool res = true;

		auto texSize = material->GetTextureCount(texType);
		int indexOffset = startIndex;
		LIGHT_CONTRIBUTION contribution = LIGHT_CONTRIBUTION::AMBIENT;

		switch (texType) {
		case aiTextureType_DIFFUSE:
			contribution = LIGHT_CONTRIBUTION::DIFFUSE;
			break;
		case aiTextureType_SPECULAR:
			indexOffset = 3;
			contribution = LIGHT_CONTRIBUTION::SPECULAR;
			break;
		case aiTextureType_NORMALS:
			contribution = LIGHT_CONTRIBUTION::NORMAL;
			indexOffset = 5;
			break;
		case aiTextureType_AMBIENT:
			break;
		case aiTextureType_DISPLACEMENT:
			break;
		case aiTextureType_EMISSIVE:
			contribution = LIGHT_CONTRIBUTION::EMISSIVE;
			break;
		case aiTextureType_LIGHTMAP:
			break;
		default:
			break;
		}

		for (unsigned int i = 0; i < texSize; i++) {
			aiString path;
			if (material->GetTexture(texType, i, &path) == AI_SUCCESS) {
				std::string fullPath = path.data;
				replace(fullPath, ".jpg", ".dds");
				replace(fullPath, ".png", ".dds");
				res &= AddTexture(startIndex, fullPath, contribution);
				indexOffset++;
			}
		}
		if (!res) {
			res = AddTexture(startIndex, DefaultTexture, contribution);
		}

		glm::vec4 color(0.8f, 0.8f, 0.8f, 1.0f);
		aiColor4D diffuse;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
			memcpy(&color, &diffuse, sizeof(float) * 4);
		}
		materials[startIndex].SetDiffuseColor(color);

		color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		aiColor4D ambient;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
			memcpy(&color, &ambient, sizeof(float) * 4);
		}
		materials[startIndex].SetAmbientColor(color);

		color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D specular;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
			memcpy(&color, &specular, sizeof(float) * 4);
		materials[startIndex].SetSpecularColor(color);

		color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		aiColor4D emission;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission))
			memcpy(&color, &emission, sizeof(float) * 4);
		materials[startIndex].SetEmissiveColor(color);

		float shininess = 0.0f;
		unsigned int max;
		aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
		materials[startIndex].SetShininess(shininess);

		return res;
	}

	void Model::Render()
	{
		vao.BindVAO();
		for (auto&& mesh : children) {
			mesh->Render();
		}
	}
    
    void Model::Render(int nbInstances)
    {
        vao.BindVAO();
        for (auto&& mesh : children) {
            mesh->Render(nbInstances);
        }
    }
}
