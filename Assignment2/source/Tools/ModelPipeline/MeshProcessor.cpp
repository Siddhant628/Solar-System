#include "pch.h"

using namespace std;
using namespace DirectX;
using namespace Library;

namespace ModelPipeline
{
	shared_ptr<Library::Mesh> MeshProcessor::LoadMesh(Library::Model& model, aiMesh& mesh)
	{
		MeshData meshData;

		meshData.Material = model.Materials().at(mesh.mMaterialIndex);

		// Vertices
		meshData.Vertices.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++)
		{
			meshData.Vertices.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mVertices[i])));
		}

		// Normals
		if (mesh.HasNormals())
		{
			meshData.Normals.reserve(mesh.mNumVertices);
			for (UINT i = 0; i < mesh.mNumVertices; i++)
			{
				meshData.Normals.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
			}
		}

		// Tangents and Binormals
		if (mesh.HasTangentsAndBitangents())
		{
			meshData.Tangents.reserve(mesh.mNumVertices);
			meshData.BiNormals.reserve(mesh.mNumVertices);
			for (UINT i = 0; i < mesh.mNumVertices; i++)
			{
				meshData.Tangents.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mTangents[i])));
				meshData.BiNormals.push_back(XMFLOAT3(reinterpret_cast<const float*>(&mesh.mBitangents[i])));
			}
		}

		// Texture Coordinates
		UINT uvChannelCount = mesh.GetNumUVChannels();
		for (UINT i = 0; i < uvChannelCount; i++)
		{
			vector<XMFLOAT3>* textureCoordinates = new vector<XMFLOAT3>();
			textureCoordinates->reserve(mesh.mNumVertices);
			meshData.TextureCoordinates.push_back(textureCoordinates);

			aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
			for (UINT j = 0; j < mesh.mNumVertices; j++)
			{
				textureCoordinates->push_back(XMFLOAT3(reinterpret_cast<const float*>(&aiTextureCoordinates[j])));
			}
		}

		// Vertex Colors
		UINT colorChannelCount = mesh.GetNumColorChannels();
		for (UINT i = 0; i < colorChannelCount; i++)
		{
			vector<XMFLOAT4>* vertexColors = new vector<XMFLOAT4>();
			vertexColors->reserve(mesh.mNumVertices);
			meshData.VertexColors.push_back(vertexColors);

			aiColor4D* aiVertexColors = mesh.mColors[i];
			for (UINT j = 0; j < mesh.mNumVertices; j++)
			{
				vertexColors->push_back(XMFLOAT4(reinterpret_cast<const float*>(&aiVertexColors[j])));
			}
		}

		// Faces (note: could pre-reserve if we limit primitive types)
		if (mesh.HasFaces())
		{
			meshData.FaceCount = mesh.mNumFaces;
			for (UINT i = 0; i < meshData.FaceCount; i++)
			{
				aiFace* face = &mesh.mFaces[i];

				for (UINT j = 0; j < face->mNumIndices; j++)
				{
					meshData.Indices.push_back(face->mIndices[j]);
				}
			}
		}

		return make_shared<Library::Mesh>(model, move(meshData));
	}
}