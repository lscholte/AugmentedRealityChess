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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <gl/glew.h>

#include <stb_image.h>

#include <iostream>

namespace
{
	unsigned int TextureFromFile(const char* path, std::string const& directory)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

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
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	std::vector<Chess::ArView::Texture> loadMaterialTextures(aiMaterial* mat, std::string const& directory, aiTextureType type, std::string typeName)
	{
		std::vector<Chess::ArView::Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			Chess::ArView::Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
		}
		return textures;
	}

	std::shared_ptr<Chess::ArView::DrawableObject> processMesh(aiMesh* pMesh, const aiScene* pScene, std::string const& directory)
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

			// texture coordinates
			if (pMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = pMesh->mTextureCoords[0][i].x;
				vec.y = pMesh->mTextureCoords[0][i].y;
				vertexBuilder.addTextureCoord(vec);
				//// tangent
				//vector.x = mesh->mTangents[i].x;
				//vector.y = mesh->mTangents[i].y;
				//vector.z = mesh->mTangents[i].z;
				//vertex.Tangent = vector;
				//// bitangent
				//vector.x = mesh->mBitangents[i].x;
				//vector.y = mesh->mBitangents[i].y;
				//vector.z = mesh->mBitangents[i].z;
				//vertex.Bitangent = vector;
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

		// process materials
		aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];

		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<Chess::ArView::Texture> diffuseMaps = loadMaterialTextures(material, directory, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//// 2. specular maps
		//std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//// 3. normal maps
		//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		//// 4. height maps
		//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		return std::make_shared<Chess::ArView::TriangleMesh>(vertices, indices, textures);
	}

	std::shared_ptr<Chess::ArView::DrawableObject> processNode(
		aiNode* node,
		const aiScene* scene,
		std::string const& directory)
	{
		std::vector<std::shared_ptr<Chess::ArView::DrawableObject>> drawableObjects;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			drawableObjects.push_back(processMesh(mesh, scene, directory));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			drawableObjects.push_back(processNode(node->mChildren[i], scene, directory));
		}
		return std::make_shared<Chess::ArView::CompositeDrawableObject>(drawableObjects);
	}
}

namespace Chess
{
namespace ArView
{
	ObjectLoader::~ObjectLoader() = default;

	//https://learnopengl.com/Model-Loading/Assimp
	std::shared_ptr<DrawableObject> ObjectLoader::load(std::string const& filePath)
	{
		Assimp::Importer importer;
		aiScene const* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!pScene)
		{
			return nullptr;
		}

		std::string directory = filePath.substr(0, filePath.find_last_of('/'));
		return processNode(pScene->mRootNode, pScene, directory);
	}

}
}
