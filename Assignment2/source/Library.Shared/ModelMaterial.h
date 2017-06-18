#pragma once

#include <string>
#include <map>
#include <vector>

namespace Library
{
	class Model;
	class OutputStreamHelper;
	class InputStreamHelper;

    enum class TextureType
    {
        Diffuse = 0,
        SpecularMap,
        Ambient,
        Emissive,
        Heightmap,
        NormalMap,
        SpecularPowerMap,
        DisplacementMap,
        LightMap,
        End
    };

	struct ModelMaterialData
	{
		std::string Name;
		std::map<TextureType, std::vector<std::string>*> Textures;

		ModelMaterialData() = default;
		ModelMaterialData(const ModelMaterialData&) = delete;
		ModelMaterialData& operator=(const ModelMaterialData&) = delete;
		ModelMaterialData(ModelMaterialData&& rhs);
		ModelMaterialData& operator=(ModelMaterialData&& rhs);
		~ModelMaterialData();

	private:
		void Clear();
	};

    class ModelMaterial
    {
    public:
		ModelMaterial(Model& model, InputStreamHelper& streamHelper);
		ModelMaterial(Model& model, ModelMaterialData&& modelMaterialData);
		ModelMaterial(ModelMaterial&& rhs);
		ModelMaterial& operator=(ModelMaterial&& rhs);

        Model& GetModel();
        const std::string& Name() const;
        const std::map<TextureType, std::vector<std::string>*>& Textures() const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        Model* mModel;
		ModelMaterialData mData;
    };
}