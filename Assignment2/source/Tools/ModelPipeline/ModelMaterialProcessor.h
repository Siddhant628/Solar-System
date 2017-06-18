#pragma once

#include <memory>

struct aiMaterial;

namespace Library
{
	class Model;
	class ModelMaterial;
}

namespace ModelPipeline
{
    class ModelMaterialProcessor
    {
    public:
		ModelMaterialProcessor() = delete;
		
		static std::shared_ptr<Library::ModelMaterial> LoadModelMaterial(Library::Model& model, aiMaterial& material);

	private:
        static void InitializeTextureTypeMappings();
        static std::map<Library::TextureType, UINT> sTextureTypeMappings;
    };
}