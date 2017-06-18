#include "pch.h"

namespace Library
{
	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates::AlphaBlending;
	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendStates::MultiplicativeBlending;

	void BlendStates::Initialize(ID3D11Device* direct3DDevice)
	{
		assert(direct3DDevice != nullptr);

		D3D11_BLEND_DESC blendStateDesc = { 0 };
		blendStateDesc.RenderTarget[0].BlendEnable = true;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ThrowIfFailed(direct3DDevice->CreateBlendState(&blendStateDesc, AlphaBlending.GetAddressOf()), "ID3D11Device::CreateBlendState() failed.");

		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		blendStateDesc.RenderTarget[0].BlendEnable = true;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		ThrowIfFailed(direct3DDevice->CreateBlendState(&blendStateDesc, MultiplicativeBlending.GetAddressOf()), "ID3D11Device::CreateBlendState() failed.");
	}

	void BlendStates::Shutdown()
	{
		AlphaBlending = nullptr;
		MultiplicativeBlending = nullptr;
	}
}