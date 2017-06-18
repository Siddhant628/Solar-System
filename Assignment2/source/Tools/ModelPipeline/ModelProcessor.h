#pragma once

#include <string>
#include "Model.h"

struct aiNode;

namespace Library
{
	struct ModelData;
	class SceneNode;
}

namespace ModelPipeline
{
    class ModelProcessor
    {
    public:
		ModelProcessor() = delete;

		static Library::Model LoadModel(const std::string& filename, bool flipUVs = false);
    };
}
