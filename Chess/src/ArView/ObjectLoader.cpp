// Author:	Liam Scholte
// Created:	3/12/2022 9:52:46 AM
// This file contains the implementations for ObjectLoader
// See ObjectLoader.h for documentation

#include <Chess/ArView/ObjectLoader.h>
#include <Chess/ArView/Vertex.h>
#include <Chess/ArView/DrawableObject.h>
#include <Chess/ArView/TriangleMesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

namespace
{
	std::shared_ptr<Chess::ArView::DrawableObject> processMesh(aiMesh* pMesh, const aiScene* pScene, glm::vec3 const& color)
	{
		std::vector<Chess::ArView::Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			Chess::ArView::Vertex vertex;
			aiVector3D position = pMesh->mVertices[i];
			vertex.position = glm::vec3(position.x, position.y, position.z);

			aiVector3D normal = pMesh->mNormals[i];
			vertex.normal = glm::vec3(normal.x, normal.y, normal.z);

			//Setting a hardcoded color
			//is basically a lazy placeholder for not trying
			//to load textures.
			vertex.color = color;

			vertices.push_back(std::move(vertex));
		}
		for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
		{
			aiFace face = pMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		return std::make_shared<Chess::ArView::TriangleMesh>(vertices, indices);
	}

	void processNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Chess::ArView::DrawableObject>>& whiteKnight, glm::vec3 const& color)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			whiteKnight.push_back(processMesh(mesh, scene, color));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, whiteKnight, color);
		}
	}
}

namespace Chess
{
namespace ArView
{
	ObjectLoader::~ObjectLoader() = default;

	//https://learnopengl.com/Model-Loading/Assimp
	bool ObjectLoader::load(
		std::string const& filePath,
		std::vector<std::shared_ptr<DrawableObject>>&whiteKnight,
		glm::vec3 const& color)
	{
		Assimp::Importer importer;
		aiScene const* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!pScene)
		{
			return false;
		}

		whiteKnight.clear();
		processNode(pScene->mRootNode, pScene, whiteKnight, color);

		return true;
	}

}
}