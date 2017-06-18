#include "pch.h"

using namespace std;
using namespace Library;

namespace ModelPipeline
{
	map<TextureType, UINT> ModelMaterialProcessor::sTextureTypeMappings;

	shared_ptr<ModelMaterial> ModelMaterialProcessor::LoadModelMaterial(Model& model, aiMaterial& material)
    {
        InitializeTextureTypeMappings();
		ModelMaterialData modelMaterialData;

        aiString name;
        material.Get(AI_MATKEY_NAME, name);
        modelMaterialData.Name = name.C_Str();

        for (TextureType textureType = (TextureType)0; textureType < TextureType::End; textureType = (TextureType)(static_cast<int>(textureType) + 1))
        {
            aiTextureType mappedTextureType = (aiTextureType)sTextureTypeMappings[textureType];

            UINT textureCount = material.GetTextureCount(mappedTextureType);
            if (textureCount > 0)
            {
                vector<string>* textures = new vector<string>();
				modelMaterialData.Textures.insert(pair<TextureType, vector<string>*>(textureType, textures));

                textures->reserve(textureCount);
                for (UINT textureIndex = 0; textureIndex < textureCount; textureIndex++)
                {
                    aiString path;
                    if (material.GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
                    {
                        textures->push_back(path.C_Str());
                    }
                }
            }
        }

		return make_shared<ModelMaterial>(model, move(modelMaterialData));
    }

	void ModelMaterialProcessor::InitializeTextureTypeMappings()
    {
		if (sTextureTypeMappings.size() != static_cast<int>(TextureType::End))
        {
            sTextureTypeMappings[TextureType::Diffuse] = aiTextureType_DIFFUSE;
			sTextureTypeMappings[TextureType::SpecularMap] = aiTextureType_SPECULAR;
			sTextureTypeMappings[TextureType::Ambient] = aiTextureType_AMBIENT;
			sTextureTypeMappings[TextureType::Heightmap] = aiTextureType_HEIGHT;
			sTextureTypeMappings[TextureType::NormalMap] = aiTextureType_NORMALS;
			sTextureTypeMappings[TextureType::SpecularPowerMap] = aiTextureType_SHININESS;
			sTextureTypeMappings[TextureType::DisplacementMap] = aiTextureType_DISPLACEMENT;
			sTextureTypeMappings[TextureType::LightMap] = aiTextureType_LIGHTMAP;
        }
    }
}