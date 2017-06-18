#include "pch.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace Library
{
	std::random_device ColorHelper::sDevice;
	std::default_random_engine ColorHelper::sGenerator(sDevice());
	std::uniform_real_distribution<float> ColorHelper::sDistribution(0, 1);

	XMFLOAT4 ColorHelper::RandomColor()
	{
		float r = sDistribution(sGenerator);
		float g = sDistribution(sGenerator);
		float b = sDistribution(sGenerator);

		return XMFLOAT4(r, g, b, 1.0f);
	}

	XMFLOAT4 ColorHelper::ToFloat4(const XMCOLOR& color, bool normalize)
	{
		return (normalize ? XMFLOAT4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f) : XMFLOAT4(color.r, color.g, color.b, color.a));
	}

	XMFLOAT3 ColorHelper::ToFloat3(const XMCOLOR& color, bool normalize)
	{
		return (normalize ? XMFLOAT3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f) : XMFLOAT3(color.r, color.g, color.b));
	}
}
