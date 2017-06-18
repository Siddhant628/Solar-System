#include "pch.h"

using namespace std;
using namespace DirectX;

namespace Library
{
#pragma region ModelData

	ModelData::ModelData(ModelData&& rhs) :
		Meshes(move(rhs.Meshes)), Materials(move(rhs.Materials))
	{
	}

	ModelData& ModelData::operator=(ModelData&& rhs)
	{
		if (this != &rhs)
		{
			Meshes = move(rhs.Meshes);
			Materials = move(rhs.Materials);
		}

		return *this;
	}

#pragma endregion

	Model::Model(const string& filename)
	{
		Load(filename);
	}

	Model::Model(ifstream& file)
	{
		Load(file);
	}

	Model::Model(ModelData&& modelData) :
		mData(move(modelData))
	{
	}

	Model::Model(Model&& rhs) :
		mData(move(rhs.mData))
	{
	}

	Model& Model::operator=(Model&& rhs)
	{
		if (this != &rhs)
		{
			mData = move(rhs.mData);
		}

		return *this;
	}

	bool Model::HasMeshes() const
	{
		return (mData.Meshes.size() > 0);
	}

	bool Model::HasMaterials() const
	{
		return (mData.Materials.size() > 0);
	}

	const vector<shared_ptr<Mesh>>& Model::Meshes() const
	{
		return mData.Meshes;
	}

	const vector<shared_ptr<ModelMaterial>>& Model::Materials() const
	{
		return mData.Materials;
	}

	ModelData& Model::Data()
	{
		return mData;
	}

	void Model::Save(const string& filename) const
	{
		ofstream file(filename.c_str(), ios::binary);
		if (!file.good())
		{
			throw exception("Could not open file.");
		}

		Save(file);
	}

	void Model::Save(ofstream& file) const
	{
		OutputStreamHelper streamHelper(file);

		// Serialize materials
		streamHelper << static_cast<uint32_t>(mData.Materials.size());
		for (const auto& material : mData.Materials)
		{
			material->Save(streamHelper);
		}

		// Serialize meshes
		streamHelper << static_cast<uint32_t>(mData.Meshes.size());
		for (auto& mesh : mData.Meshes)
		{
			mesh->Save(streamHelper);
		}
	}

	void Model::Load(const string& filename)
	{
		ifstream file(filename.c_str(), ios::binary);
		if (!file.good())
		{
			throw GameException("Could not open file.");
		}

		Load(file);
	}

	void Model::Load(ifstream& file)
	{
		InputStreamHelper streamHelper(file);

		// Desrialize materials
		uint32_t materialCount;
		streamHelper >> materialCount;
		mData.Materials.reserve(materialCount);
		for (uint32_t i = 0; i < materialCount; i++)
		{
			mData.Materials.push_back(make_shared<ModelMaterial>(*this, streamHelper));
		}

		// Desrialize meshes
		uint32_t meshCount;
		streamHelper >> meshCount;
		mData.Meshes.reserve(meshCount);
		for (uint32_t i = 0; i < meshCount; i++)
		{
			mData.Meshes.push_back(make_shared<Mesh>(*this, streamHelper));
		}
	}
}