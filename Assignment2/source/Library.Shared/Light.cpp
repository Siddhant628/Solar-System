#include "pch.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace Library
{
	RTTI_DEFINITIONS(Light)

	Light::Light(Game& game) :
		GameComponent(game), mColor(reinterpret_cast<const float*>(&Colors::White))
	{
	}

	const XMCOLOR& Light::Color() const
	{
		return mColor;
	}

	XMVECTOR Light::ColorVector() const
	{
		return XMLoadColor(&mColor);
	}

	void Light::SetColor(float r, float g, float b, float a)
	{
		SetColor(XMCOLOR(r, g, b, a));
	}

	void Light::SetColor(const XMFLOAT4& color)
	{
		SetColor(XMCOLOR(color.x, color.y, color.z, color.w));
	}

	void Light::SetColor(const XMCOLOR& color)
	{
		mColor = color;
	}

	void Light::SetColor(FXMVECTOR color)
	{
		XMStoreColor(&mColor, color);
	}
}