#pragma once

#include <memory>

struct aiMesh;

namespace Library
{
	class Model;
	class Mesh;
}

namespace ModelPipeline
{
    class MeshProcessor
    {
    public:
		MeshProcessor() = delete;

		static std::shared_ptr<Library::Mesh> LoadMesh(Library::Model& model, aiMesh& mesh);
    };
}