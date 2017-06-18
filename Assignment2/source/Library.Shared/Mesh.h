#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <DirectXMath.h>
#include <d3d11_2.h>

namespace Library
{
	class Model;
    class Material;
    class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct MeshData
	{
		std::shared_ptr<ModelMaterial> Material;
		std::string Name;
		std::vector<DirectX::XMFLOAT3> Vertices;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT3> Tangents;
		std::vector<DirectX::XMFLOAT3> BiNormals;
		std::vector<std::vector<DirectX::XMFLOAT3>*> TextureCoordinates;
		std::vector<std::vector<DirectX::XMFLOAT4>*> VertexColors;
		std::uint32_t FaceCount;
		std::vector<std::uint32_t> Indices;

		MeshData();
		MeshData(const MeshData&) = delete;
		MeshData& operator=(const MeshData&) = delete;
		MeshData(MeshData&& rhs);
		MeshData& operator=(MeshData&& rhs);
		~MeshData();

	private:
		void Clear();
	};

    class Mesh
    {
    public:
		Mesh(Library::Model& model, InputStreamHelper& streamHelper);
		Mesh(Library::Model& model, MeshData&& meshData);
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& rhs);
		Mesh& operator=(Mesh&& rhs);

		Library::Model& GetModel();
        std::shared_ptr<ModelMaterial> GetMaterial();
        const std::string& Name() const;

		const std::vector<DirectX::XMFLOAT3>& Vertices() const;
		const std::vector<DirectX::XMFLOAT3>& Normals() const;
		const std::vector<DirectX::XMFLOAT3>& Tangents() const;
		const std::vector<DirectX::XMFLOAT3>& BiNormals() const;
		const std::vector<std::vector<DirectX::XMFLOAT3>*>& TextureCoordinates() const;
		const std::vector<std::vector<DirectX::XMFLOAT4>*>& VertexColors() const;
		std::uint32_t FaceCount() const;
		const std::vector<std::uint32_t>& Indices() const;

        void CreateIndexBuffer(ID3D11Device& device, ID3D11Buffer** indexBuffer);
		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        Library::Model* mModel;
		MeshData mData;
    };
}