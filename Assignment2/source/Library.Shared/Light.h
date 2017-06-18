#pragma once

#include "GameComponent.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace Library
{
	class Light : public GameComponent
	{
		RTTI_DECLARATIONS(Light, GameComponent)

	public:
		Light(Game& game);
		Light(const Light&) = default;
		Light& operator=(const Light&) = default;
		Light& operator=(Light&&) = default;
		Light(Light&&) = default;
		virtual ~Light() = default;

		const DirectX::PackedVector::XMCOLOR& Color() const;
		DirectX::XMVECTOR ColorVector() const;
		void SetColor(float r, float g, float b, float a);
		void SetColor(const DirectX::XMFLOAT4& color);
		void SetColor(const DirectX::PackedVector::XMCOLOR& color);
		void SetColor(DirectX::FXMVECTOR color);

	protected:
		DirectX::PackedVector::XMCOLOR mColor;
	};
}

