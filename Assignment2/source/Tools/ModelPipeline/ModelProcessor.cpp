#include "pch.h"

using namespace std;
using namespace Library;
using namespace DirectX;

namespace ModelPipeline
{
	Library::Model ModelProcessor::LoadModel(const std::string& filename, bool flipUVs)
	{
		Library::Model model;
		ModelData& modelData = model.Data();
		Assimp::Importer importer;

		UINT flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;
		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}

		const aiScene* scene = importer.ReadFile(filename, flags);
		if (scene == nullptr)
		{
			throw exception(importer.GetErrorString());
		}

		if (scene->HasMaterials())
		{
			for (UINT i = 0; i < scene->mNumMaterials; i++)
			{
				shared_ptr<ModelMaterial> modelMaterial = ModelMaterialProcessor::LoadModelMaterial(model, *(scene->mMaterials[i]));
				modelData.Materials.push_back(modelMaterial);
			}
		}

		if (scene->HasMeshes())
		{
			for (UINT i = 0; i < scene->mNumMeshes; i++)
			{
				shared_ptr<Mesh> mesh = MeshProcessor::LoadMesh(model, *(scene->mMeshes[i]));
				modelData.Meshes.push_back(mesh);
			}
		}

		return model;
	}
}