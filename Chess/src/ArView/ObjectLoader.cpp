// Author:	Liam Scholte
// Created:	3/12/2022 9:52:46 AM
// This file contains the implementations for ObjectLoader
// See ObjectLoader.h for documentation

#include <Chess/ArView/ObjectLoader.h>
#include <Chess/ArView/Vertex.h>
#include <Chess/ArView/VertexBuilder.h>
#include <Chess/ArView/DrawableObject.h>
#include <Chess/ArView/CompositeDrawableObject.h>
#include <Chess/ArView/TriangleMesh.h>
#include <Chess/ArView/Texture.h>
#include <Chess/ArView/Constants.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gl/glew.h>

#include <stb_image.h>

#include <iostream>
#include <filesystem>

namespace
{
	GLenum colorComponentCountToFormat(int numComponents)
	{
		switch (numComponents)
		{
		case 1: return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}
		throw std::runtime_error("Invalid number of components");
	}

	unsigned int generateOpenGlTexture(std::filesystem::path const& texturePath)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, numComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(texturePath.string().c_str(), &width, &height, &numComponents, 0);
		if (data)
		{
			GLenum format = colorComponentCountToFormat(numComponents);

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << texturePath << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	std::string getTextureTypeString(aiTextureType type)
	{
		switch (type)
		{
		case aiTextureType_DIFFUSE: return Chess::ArView::TEXTURE_DIFFUSE;
		}
		throw std::runtime_error("Unhandled texture type");
	}
}

namespace Chess
{
namespace ArView
{
	struct ObjectLoader::Impl
	{
		std::vector<Texture> loadedTextures;

		std::vector<Chess::ArView::Texture> loadMaterialTextures(
			aiMaterial* pMaterial,
			std::filesystem::path const& directory,
			aiTextureType textureType)
		{
			std::vector<Chess::ArView::Texture> textures;
			for (unsigned int i = 0; i < pMaterial->GetTextureCount(textureType); ++i)
			{
				aiString textureFileName;
				pMaterial->GetTexture(textureType, i, &textureFileName);

				std::filesystem::path texturePath = directory / textureFileName.C_Str();

				//Check previously loaded textures for the texture we want
				bool skip = false;
				for (unsigned int j = 0; j < loadedTextures.size(); ++j)
				{
					if (loadedTextures[j].path == texturePath)
					{
						textures.push_back(loadedTextures[j]);
						skip = true;
						break;
					}
				}

				//Load the texture only if a texture at the particular path
				//has not already been loaded
				if (!skip)
				{
					Chess::ArView::Texture texture;
					texture.id = generateOpenGlTexture(texturePath);
					texture.type = getTextureTypeString(textureType);
					texture.path = texturePath;
					textures.push_back(texture);

					//Add the texture to the cached list of textures for later use
					loadedTextures.push_back(texture);
				}
			}
			return textures;
		}

		std::shared_ptr<Chess::ArView::DrawableObject> processMesh(
			aiMesh* pMesh,
			const aiScene* pScene,
			std::filesystem::path const& directory)
		{
			std::vector<Chess::ArView::Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<Chess::ArView::Texture> textures;

			for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
			{
				Chess::ArView::VertexBuilder vertexBuilder;

				aiVector3D position = pMesh->mVertices[i];
				vertexBuilder.addPosition(glm::vec3(position.x, position.y, position.z));

				aiVector3D normal = pMesh->mNormals[i];
				vertexBuilder.addNormal(glm::vec3(normal.x, normal.y, normal.z));

				//Vertices may have up to 8 texture coordinates.
				//See if the first set of texture coordinates exists
				//and ignore the remaining 7 sets of coordinates.
				if (pMesh->mTextureCoords[0])
				{
					aiVector3D textureCoord = pMesh->mTextureCoords[0][i];
					vertexBuilder.addTextureCoord(glm::vec2(textureCoord.x, textureCoord.y));
				}

				vertices.push_back(vertexBuilder.build());
			}
			for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
			{
				aiFace face = pMesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; ++j)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];

			std::vector<Chess::ArView::Texture> diffuseMaps = loadMaterialTextures(pMaterial, directory, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			return std::make_shared<Chess::ArView::TriangleMesh>(vertices, indices, textures);
		}

		std::shared_ptr<Chess::ArView::DrawableObject> processNode(
			aiNode* pNode,
			const aiScene* scene,
			std::filesystem::path const& directory)
		{
			std::vector<std::shared_ptr<Chess::ArView::DrawableObject>> drawableObjects;
			for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[pNode->mMeshes[i]];
				drawableObjects.push_back(processMesh(mesh, scene, directory));
			}
			for (unsigned int i = 0; i < pNode->mNumChildren; i++)
			{
				drawableObjects.push_back(processNode(pNode->mChildren[i], scene, directory));
			}
			return std::make_shared<Chess::ArView::CompositeDrawableObject>(drawableObjects);
		}
	};

	ObjectLoader::ObjectLoader()
		: m_pImpl(std::make_unique<Impl>())
	{}

	ObjectLoader::~ObjectLoader() = default;

	//https://learnopengl.com/Model-Loading/Assimp
	std::shared_ptr<DrawableObject> ObjectLoader::load(std::filesystem::path const& filePath)
	{
		Assimp::Importer importer;
		aiScene const* pScene = importer.ReadFile(filePath.string(), aiProcess_Triangulate | aiProcess_GenNormals);
		if (!pScene)
		{
			return nullptr;
		}

		return m_pImpl->processNode(pScene->mRootNode, pScene, filePath.parent_path());
	}

}
}
