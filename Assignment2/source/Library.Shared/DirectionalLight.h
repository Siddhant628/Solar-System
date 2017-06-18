#pragma once

#include "Light.h"

namespace Library
{
	class DirectionalLight : public Light
	{
		RTTI_DECLARATIONS(DirectionalLight, Light)

	public:
		DirectionalLight(Game& game);
		virtual ~DirectionalLight() = default;

		const DirectX::XMFLOAT3& Direction() const;
		DirectX::XMFLOAT3 DirectionToLight() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR DirectionToLightVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

	protected:
		DirectX::XMFLOAT3 mDirection;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mRight;
	};
}

