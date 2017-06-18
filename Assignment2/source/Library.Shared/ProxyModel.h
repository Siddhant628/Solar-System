#pragma once

#include "DrawableGameComponent.h"
#include <wrl.h>
#include <d3d11_2.h>
#include <DirectXMath.h>

namespace Library
{
	class Mesh;

	class ProxyModel : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ProxyModel, DrawableGameComponent)

	public:
		ProxyModel(Game& game, const std::shared_ptr<Camera>& camera, const std::string& modelFileName, float scale = 1.0f);
		ProxyModel(const ProxyModel&) = delete;
		ProxyModel& operator=(const ProxyModel&) = delete;
		ProxyModel(ProxyModel&&) = delete;
		ProxyModel& operator=(ProxyModel&&) = delete;
		~ProxyModel() = default;

		const DirectX::XMFLOAT3& Position() const;
		const DirectX::XMFLOAT3& Direction() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR PositionVector() const;
		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		bool& DisplayWireframe();

		void SetPosition(FLOAT x, FLOAT y, FLOAT z);
		void SetPosition(DirectX::FXMVECTOR position);
		void SetPosition(const DirectX::XMFLOAT3& position);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;		
		virtual void Draw(const GameTime& gameTime) override;

	private:
		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;

			VertexCBufferPerObject() : WorldViewProjection() { }

			VertexCBufferPerObject(const DirectX::XMFLOAT4X4& wvp) : WorldViewProjection(wvp) { }
		};

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

		DirectX::XMFLOAT4X4 mWorldMatrix;
		DirectX::XMFLOAT4X4 mScaleMatrix;
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mDirection;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mRight;
		std::string mModelFileName;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;		
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexCBufferPerObject;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		UINT mIndexCount;
		bool mDisplayWireframe;
	};
}