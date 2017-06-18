#pragma once

#include "PointLight.h"

namespace Library
{
	class SpotLight : public PointLight
	{
		RTTI_DECLARATIONS(SpotLight, PointLight)

	public:
		SpotLight(Game& game, const DirectX::XMFLOAT3& position = Vector3Helper::Zero, float radius = DefaultRadius);
		virtual ~SpotLight() = default;
	
		const DirectX::XMFLOAT3& Direction() const;
		DirectX::XMFLOAT3 DirectionToLight() const;
		const DirectX::XMFLOAT3& Up() const;
		const DirectX::XMFLOAT3& Right() const;

		DirectX::XMVECTOR DirectionVector() const;
		DirectX::XMVECTOR DirectionToLightVector() const;
		DirectX::XMVECTOR UpVector() const;
		DirectX::XMVECTOR RightVector() const;

		float InnerAngle();
		void SetInnerAngle(float value);
		
		float OuterAngle();
		void SetOuterAngle(float value);

		void ApplyRotation(DirectX::CXMMATRIX transform);
		void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

		static const float DefaultInnerAngle;
		static const float DefaultOuterAngle;

	protected:
		DirectX::XMFLOAT3 mDirection;
		DirectX::XMFLOAT3 mUp;
		DirectX::XMFLOAT3 mRight;
		float mInnerAngle;
		float mOuterAngle;
	};
}