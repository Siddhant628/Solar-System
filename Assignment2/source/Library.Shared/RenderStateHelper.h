#pragma once

#include <wrl.h>
#include <d3d11_2.h>

namespace Library
{
	class Game;

	class RenderStateHelper final
	{
	public:
		RenderStateHelper(Game& game);
		RenderStateHelper(const RenderStateHelper&) = delete;
		RenderStateHelper& operator=(const RenderStateHelper&) = delete;
		RenderStateHelper(RenderStateHelper&&) = delete;
		RenderStateHelper& operator=(RenderStateHelper&&) = delete;
		~RenderStateHelper();

		static void ResetAll(ID3D11DeviceContext* deviceContext);
		static void ResetRasterizerState(ID3D11DeviceContext* deviceContext);
		static void ResetBlendState(ID3D11DeviceContext* deviceContext);
		static void ResetDepthStencilState(ID3D11DeviceContext* deviceContext);

		ID3D11RasterizerState* RasterizerState();
		ID3D11BlendState* BlendState();
		ID3D11DepthStencilState* DepthStencilState();

		void SaveRasterizerState();
		void RestoreRasterizerState() const;

		void SaveBlendState();
		void RestoreBlendState() const;

		void SaveDepthStencilState();
		void RestoreDepthStencilState() const;

		void SaveAll();
		void RestoreAll() const;

	private:
		Game& mGame;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mBlendState;
		FLOAT* mBlendFactor;
		UINT mSampleMask;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;
		UINT mStencilRef;
	};
}
