#pragma once

#include "DrawableGameComponent.h"
#include <wrl.h>
#include <d3d11_2.h>
#include <DirectXMath.h>

namespace Library
{
	class Mesh;

	class Skybox final : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Skybox, DrawableGameComponent)

	public:
		Skybox() = delete;
		Skybox(Game& game, const std::shared_ptr<Camera>& camera, const std::wstring& cubeMapFileName, float scale);
		Skybox(const Skybox&) = delete;
		Skybox& operator=(const Skybox&) = delete;
		Skybox(Skybox&&) = delete;
		Skybox& operator=(Skybox&&) = delete;
		~Skybox() = default;

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VertexCBufferPerObject() { }
			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

		DirectX::XMFLOAT4X4 mWorldMatrix;
		DirectX::XMFLOAT4X4 mScaleMatrix;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		std::wstring mCubeMapFileName;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;		
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSkyboxTexture;
		UINT mIndexCount;		
	};
}