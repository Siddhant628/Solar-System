#pragma once

#include "RTTI.h"
#include <d3d11_2.h>
#include <stack>

namespace Library
{
	class RenderTarget : public RTTI
	{
		RTTI_DECLARATIONS(RenderTarget, RTTI)

	public:
		RenderTarget() = default;
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = delete;
		RenderTarget& operator=(RenderTarget&&) = delete;
		virtual ~RenderTarget() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;

	protected:
		struct RenderTargetData
		{
			UINT ViewCount;
			ID3D11RenderTargetView** RenderTargetViews;
			ID3D11DepthStencilView* DepthStencilView;
			D3D11_VIEWPORT Viewport;

			RenderTargetData(UINT viewCount, ID3D11RenderTargetView** renderTargetViews, ID3D11DepthStencilView* depthStencilView, const D3D11_VIEWPORT& viewport) :
				ViewCount(viewCount), RenderTargetViews(renderTargetViews), DepthStencilView(depthStencilView), Viewport(viewport) { }
		};

		void Begin(ID3D11DeviceContext* deviceContext, UINT viewCount, ID3D11RenderTargetView** renderTargetViews, ID3D11DepthStencilView* depthStencilView, const D3D11_VIEWPORT& viewport);
		void End(ID3D11DeviceContext* deviceContext);
		void RebindCurrentRenderTargets(ID3D11DeviceContext* deviceContext);

	private:
		static std::stack<RenderTargetData> sRenderTargetStack;
	};
}