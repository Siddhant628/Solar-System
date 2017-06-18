#include "pch.h"

namespace Library
{
	RenderStateHelper::RenderStateHelper(Game& game) :
		mGame(game), mBlendFactor(new FLOAT[4]), mSampleMask(UINT_MAX), mStencilRef(UINT_MAX)
	{
	}

	RenderStateHelper::~RenderStateHelper()
	{
		DeleteObjects(mBlendFactor);
	}

	void RenderStateHelper::ResetAll(ID3D11DeviceContext* deviceContext)
	{
		ResetRasterizerState(deviceContext);
		ResetBlendState(deviceContext);
		ResetDepthStencilState(deviceContext);
	}

	void RenderStateHelper::ResetRasterizerState(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->RSSetState(nullptr);
	}

	void RenderStateHelper::ResetBlendState(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
	}

	void RenderStateHelper::ResetDepthStencilState(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->OMSetDepthStencilState(nullptr, UINT_MAX);
	}

	ID3D11RasterizerState* RenderStateHelper::RasterizerState()
	{
		return mRasterizerState.Get();
	}

	ID3D11BlendState* RenderStateHelper::BlendState()
	{
		return mBlendState.Get();
	}

	ID3D11DepthStencilState* RenderStateHelper::DepthStencilState()
	{
		return mDepthStencilState.Get();
	}

	void RenderStateHelper::SaveRasterizerState()
	{
		mGame.Direct3DDeviceContext()->RSGetState(mRasterizerState.ReleaseAndGetAddressOf());
	}

	void RenderStateHelper::RestoreRasterizerState() const
	{
		mGame.Direct3DDeviceContext()->RSSetState(mRasterizerState.Get());
	}

	void RenderStateHelper::SaveBlendState()
	{
		mGame.Direct3DDeviceContext()->OMGetBlendState(mBlendState.ReleaseAndGetAddressOf(), mBlendFactor, &mSampleMask);
	}

	void RenderStateHelper::RestoreBlendState() const
	{
		mGame.Direct3DDeviceContext()->OMSetBlendState(mBlendState.Get(), mBlendFactor, mSampleMask);
	}

	void RenderStateHelper::SaveDepthStencilState()
	{
		mGame.Direct3DDeviceContext()->OMGetDepthStencilState(mDepthStencilState.ReleaseAndGetAddressOf(), &mStencilRef);
	}

	void RenderStateHelper::RestoreDepthStencilState() const
	{
		mGame.Direct3DDeviceContext()->OMSetDepthStencilState(mDepthStencilState.Get(), mStencilRef);
	}

	void RenderStateHelper::SaveAll()
	{
		SaveRasterizerState();
		SaveBlendState();
		SaveDepthStencilState();
	}

	void RenderStateHelper::RestoreAll() const
	{
		RestoreRasterizerState();
		RestoreBlendState();
		RestoreDepthStencilState();
	}
}
