#pragma once

#include <DirectXMath.h>

namespace Library
{
	struct VertexPosition
	{
		DirectX::XMFLOAT4 Position;

		VertexPosition() { }

		VertexPosition(const DirectX::XMFLOAT4& position) :
			Position(position) { }
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT4 Color;

		VertexPositionColor() { }

		VertexPositionColor(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& color) :
			Position(position), Color(color) { }
	};

	struct VertexPositionTexture
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;

		VertexPositionTexture() { }

		VertexPositionTexture(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates) :
			Position(position), TextureCoordinates(textureCoordinates) { }
	};

	struct VertexPositionSize
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 Size;

		VertexPositionSize() { }

		VertexPositionSize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& size) :
			Position(position), Size(size) { }
	};

	struct VertexPositionNormal
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT3 Normal;

		VertexPositionNormal() { }

		VertexPositionNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT3& normal) :
			Position(position), Normal(normal) { }
	};

	struct VertexPositionTextureNormal
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;

		VertexPositionTextureNormal() { }

		VertexPositionTextureNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }
	};

	struct VertexPositionTextureNormalTangent
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;

		VertexPositionTextureNormalTangent() { }

		VertexPositionTextureNormalTangent(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangent) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), Tangent(tangent) { }
	};

	struct VertexSkinnedPositionTextureNormal
	{
		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMUINT4 BoneIndices;
		DirectX::XMFLOAT4 BoneWeights;

		VertexSkinnedPositionTextureNormal() { }

		VertexSkinnedPositionTextureNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal, const DirectX::XMUINT4& boneIndices, const DirectX::XMFLOAT4& boneWeights) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), BoneIndices(boneIndices), BoneWeights(boneWeights) { }
	};
}
