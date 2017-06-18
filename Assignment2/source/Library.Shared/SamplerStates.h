#pragma once

#include <wrl.h>
#include <d3d11_2.h>
#include <DirectXMath.h>

namespace Library
{
	class SamplerStates final
	{
	public:
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> TrilinearWrap;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> TrilinearMirror;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> TrilinearClamp;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> TrilinerBorder;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> PointClamp;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> DepthMap;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> ShadowMap;
		static Microsoft::WRL::ComPtr<ID3D11SamplerState> PcfShadowMap;

		static DirectX::XMVECTORF32 BorderColor;
		static DirectX::XMVECTORF32 ShadowMapBorderColor;

		static void Initialize(ID3D11Device* direct3DDevice);
		static void Shutdown();

		SamplerStates() = delete;
		SamplerStates(const SamplerStates&) = delete;
		SamplerStates& operator=(const SamplerStates&) = delete;
		SamplerStates(SamplerStates&&) = delete;
		SamplerStates& operator=(SamplerStates&&) = delete;
		~SamplerStates() = default;
	};
}
