#pragma once

#include <windows.h>
#include <wrl.h>
#include <string>
#include <sstream>
#include <memory>

#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include "GameClock.h"
#include "GameTime.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"

namespace Library
{
	class GameComponent;

	class IDeviceNotify
	{
	public:
		virtual ~IDeviceNotify() { };

		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		IDeviceNotify() { };
	};

    class Game : public RenderTarget
    {
		RTTI_DECLARATIONS(Game, RenderTarget)

    public:
        Game(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;
		virtual ~Game() = default;

		ID3D11Device2* Direct3DDevice() const;
		ID3D11DeviceContext2* Direct3DDeviceContext() const;
		IDXGISwapChain1* SwapChain() const;
		ID3D11RenderTargetView* RenderTargetView() const;
		ID3D11DepthStencilView* DepthStencilView() const;
		SIZE RenderTargetSize() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDesc() const;
		const D3D11_VIEWPORT& Viewport() const;
		UINT MultiSamplingCount() const;
		UINT MultiSamplingQualityLevels() const;

		const std::vector<std::shared_ptr<GameComponent>>& Components() const;
		const ServiceContainer& Services() const;			

        virtual void Initialize();
		virtual void Run();
		virtual void Shutdown();        

		void UpdateRenderTargetSize();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		virtual void UnbindPixelShaderResources(UINT startSlot, UINT count);
		std::function<void*()> GetWindowCallback() const;

    protected:
		virtual void Update(const GameTime& gameTime);
		virtual void Draw(const GameTime& gameTime);
		virtual void HandleDeviceLost();

		virtual void Begin() override;
		virtual void End() override;

		virtual void CreateDeviceIndependentResources();
		virtual void CreateDeviceResources();
		virtual void CreateWindowSizeDependentResources();

		static const UINT DefaultFrameRate;
		static const UINT DefaultMultiSamplingCount;
		static const UINT DefaultBufferCount;

		Microsoft::WRL::ComPtr<ID3D11Device2> mDirect3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext2> mDirect3DDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> mSwapChain;
		D3D_FEATURE_LEVEL mFeatureLevel;

		D3D11_TEXTURE2D_DESC mBackBufferDesc;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		D3D11_VIEWPORT mViewport;

		UINT mFrameRate;
		bool mIsFullScreen;
		UINT mMultiSamplingCount;
		UINT mMultiSamplingQualityLevels;

		std::function<void*()> mGetWindow;
		std::function<void(SIZE&)> mGetRenderTargetSize;
		SIZE mRenderTargetSize;
		IDeviceNotify* mDeviceNotify;

        GameClock mGameClock;
        GameTime mGameTime;
		std::vector<std::shared_ptr<GameComponent>> mComponents;
		ServiceContainer mServices;
    };
}