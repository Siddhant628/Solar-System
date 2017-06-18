#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>

namespace Library
{
    class Game;
    class Mesh;
    class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct ModelData
	{
		std::vector<std::shared_ptr<Mesh>> Meshes;
		std::vector<std::shared_ptr<ModelMaterial>> Materials;

		ModelData() = default;
		ModelData(const ModelData&) = delete;
		ModelData& operator=(const ModelData&) = delete;
		ModelData(ModelData&& rhs);
		ModelData& operator=(ModelData&& rhs);
		~ModelData() = default;
	};

    class Model
    {
    public:
		Model() = default;
		Model(const std::string& filename);
		Model(std::ifstream& file);
		Model(ModelData&& modelData);
		Model(Model&& rhs);
		Model& operator=(Model&& rhs);

        bool HasMeshes() const;
        bool HasMaterials() const;
		
        const std::vector<std::shared_ptr<Mesh>>& Meshes() const;
		const std::vector<std::shared_ptr<ModelMaterial>>& Materials() const;

		ModelData& Data();

		void Save(const std::string& filename) const;
		void Save(std::ofstream& file) const;

    private:
		void Load(const std::string& filename);
		void Load(std::ifstream& file);

		ModelData mData;
    };
}
