#include "pch.h"

using namespace std;
using namespace Library;

#pragma region ModelMaterialData

ModelMaterialData::ModelMaterialData(ModelMaterialData&& rhs) :
	Name(move(rhs.Name)), Textures(move(rhs.Textures))
{
}

ModelMaterialData& ModelMaterialData::operator=(ModelMaterialData&& rhs)
{
	if (this != &rhs)
	{
		Clear();

		Name = move(rhs.Name);
		Textures = move(rhs.Textures);
	}

	return *this;
}

ModelMaterialData::~ModelMaterialData()
{
	Clear();
}

void ModelMaterialData::Clear()
{
	for (pair<TextureType, vector<string>*> textures : Textures)
	{
		DeleteObject(textures.second);
	}
}

#pragma endregion

ModelMaterial::ModelMaterial(Model& model, InputStreamHelper& streamHelper) :
	mModel(&model), mData()
{
	Load(streamHelper);
}

ModelMaterial::ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData) :
	mModel(&model), mData(move(modelMaterialData))
{
}

ModelMaterial::ModelMaterial(ModelMaterial&& rhs) :
	mModel(rhs.mModel), mData(move(rhs.mData))
{	
}

ModelMaterial& ModelMaterial::operator=(ModelMaterial&& rhs)
{
	if (this != &rhs)
	{
		mModel = move(rhs.mModel);
		mData = move(rhs.mData);
	}

	return *this;
}

Model& ModelMaterial::GetModel()
{
    return *mModel;
}

const string& ModelMaterial::Name() const
{
    return mData.Name;
}

const map<TextureType, vector<string>*>& ModelMaterial::Textures() const
{
	return mData.Textures;
}

void ModelMaterial::Save(OutputStreamHelper& streamHelper) const
{
	streamHelper << mData.Name;

	streamHelper << static_cast<uint32_t>(mData.Textures.size());
	for (const auto& texturePair : mData.Textures)
	{
		streamHelper << static_cast<int32_t>(texturePair.first);
		streamHelper << static_cast<uint32_t>(texturePair.second->size());
		for (const auto& texture : *(texturePair.second))
		{
			streamHelper << texture;
		}
	}
}

void ModelMaterial::Load(InputStreamHelper& streamHelper)
{
	streamHelper >> mData.Name;

	uint32_t texturesCount;
	streamHelper >> texturesCount;
	for (uint32_t i = 0; i < texturesCount; i++)
	{
		int32_t textureType;
		streamHelper >> textureType;

		vector<string>* textures = new vector<string>();
		mData.Textures.insert(pair<TextureType, vector<string>*>(TextureType(textureType), textures));

		uint32_t textureListCount;
		streamHelper >> textureListCount;
		textures->reserve(textureListCount);
		for (uint32_t j = 0; j < textureListCount; j++)
		{
			string texture;
			streamHelper >> texture;
			textures->push_back(texture);
		}	
	}
}