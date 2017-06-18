#include "pch.h"

using namespace std;
using namespace Library;
using namespace Microsoft::WRL;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Game)

	const UINT Game::DefaultFrameRate = 60;
	const UINT Game::DefaultMultiSamplingCount = 4;
	const UINT Game::DefaultBufferCount = 2;

	Game::Game(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		RenderTarget(),
		mFeatureLevel(D3D_FEATURE_LEVEL_9_1), mFrameRate(DefaultFrameRate), mIsFullScreen(false),
		mMultiSamplingCount(DefaultMultiSamplingCount), mMultiSamplingQualityLevels(0),
		mGetWindow(getWindowCallback), mGetRenderTargetSize(getRenderTargetSizeCallback)
	{
		assert(getWindowCallback != nullptr);
		assert(mGetRenderTargetSize != nullptr);

		CreateDeviceIndependentResources();
		CreateDeviceResources();
	}

	ID3D11Device2* Game::Direct3DDevice() const
	{
		return mDirect3DDevice.Get();
	}

	ID3D11DeviceContext2* Game::Direct3DDeviceContext() const
	{
		return mDirect3DDeviceContext.Get();
	}

	IDXGISwapChain1* Game::SwapChain() const
	{
		return mSwapChain.Get();
	}

	ID3D11RenderTargetView* Game::RenderTargetView() const
	{
		return mRenderTargetView.Get();
	}

	ID3D11DepthStencilView* Game::DepthStencilView() const
	{
		return mDepthStencilView.Get();
	}

	SIZE Game::RenderTargetSize() const
	{
		return mRenderTargetSize;
	}

	float Game::AspectRatio() const
	{
		return static_cast<float>(mRenderTargetSize.cx) / mRenderTargetSize.cy;
	}

	bool Game::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	const D3D11_TEXTURE2D_DESC& Game::BackBufferDesc() const
	{
		return mBackBufferDesc;
	}

	const D3D11_VIEWPORT& Game::Viewport() const
	{
		return mViewport;
	}

	UINT Game::MultiSamplingCount() const
	{
		return mMultiSamplingCount;
	}

	UINT Game::MultiSamplingQualityLevels() const
	{
		return mMultiSamplingQualityLevels;
	}

	const vector<shared_ptr<GameComponent>>& Game::Components() const
	{
		return mComponents;
	}

	const ServiceContainer& Game::Services() const
	{
		return mServices;
	}

	void Game::Initialize()
	{
		mGameClock.Reset();

		for (auto& component : mComponents)
		{
			component->Initialize();
		}
	}

	void Game::Run()
	{
		mGameClock.UpdateGameTime(mGameTime);
		Update(mGameTime);
		Draw(mGameTime);
	}

	void Game::Shutdown()
	{
		// Free up all D3D resources.
		mDirect3DDeviceContext->ClearState();
		mDirect3DDeviceContext->Flush();
		
		mComponents.clear();
		mComponents.shrink_to_fit();

		mDepthStencilView = nullptr;
		mRenderTargetView = nullptr;
		mSwapChain = nullptr;
		mDirect3DDeviceContext = nullptr;
		mDirect3DDevice = nullptr;
	}

	void Game::Update(const GameTime& gameTime)
	{
		for (auto& component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(gameTime);
			}
		}
	}

	void Game::Draw(const GameTime& gameTime)
	{
		for (auto& component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
			{
				drawableGameComponent->Draw(gameTime);
			}
		}
	}

	void Game::UpdateRenderTargetSize()
	{
		CreateWindowSizeDependentResources();
	}

	void Game::RegisterDeviceNotify(IDeviceNotify* deviceNotify)
	{
		mDeviceNotify = deviceNotify;
	}

	void Game::UnbindPixelShaderResources(UINT startSlot, UINT count)
	{
		static ID3D11ShaderResourceView* emptySRV[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		assert(count < ARRAYSIZE(emptySRV));

		mDirect3DDeviceContext->PSSetShaderResources(startSlot, count, emptySRV);
	}

	std::function<void*()> Game::GetWindowCallback() const
	{
		return mGetWindow;
	}

	void Game::Begin()
	{
		RenderTarget::Begin(mDirect3DDeviceContext.Get(), 1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get(), mViewport);
	}

	void Game::End()
	{
		RenderTarget::End(mDirect3DDeviceContext.Get());
	}

	void Game::CreateDeviceIndependentResources()
	{
	}

	void Game::CreateDeviceResources()
	{
		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		if (SdkLayersAvailable())
		{
			// If the project is in a debug build, enable debugging via SDK Layers with this flag.
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		// Create the Direct3D device object and a corresponding context.
		ComPtr<ID3D11Device> direct3DDevice;
		ComPtr<ID3D11DeviceContext> direct3DDeviceContext;
		ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, direct3DDevice.ReleaseAndGetAddressOf(), &mFeatureLevel, direct3DDeviceContext.ReleaseAndGetAddressOf()), "D3D11CreateDevice() failed");
		ThrowIfFailed(direct3DDevice.As(&mDirect3DDevice));
		ThrowIfFailed(direct3DDeviceContext.As(&mDirect3DDeviceContext));

		ThrowIfFailed(mDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMultiSamplingCount, &mMultiSamplingQualityLevels), "CheckMultisampleQualityLevels() failed.");
		if (mMultiSamplingQualityLevels == 0)
		{
			throw GameException("Unsupported multi-sampling quality");
		}

#ifndef NDEBUG
		ComPtr<ID3D11Debug> d3dDebug;
		HRESULT hr = mDirect3DDevice.As(&d3dDebug);
		if (SUCCEEDED(hr))
		{
			ComPtr<ID3D11InfoQueue> d3dInfoQueue;
			hr = d3dDebug.As(&d3dInfoQueue);
			if (SUCCEEDED(hr))
			{
#ifdef _DEBUG
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
				};
				D3D11_INFO_QUEUE_FILTER filter = { 0 };
				filter.DenyList.NumIDs = _countof(hide);
				filter.DenyList.pIDList = hide;
				d3dInfoQueue->AddStorageFilterEntries(&filter);
			}
		}
#endif
	}

	void Game::CreateWindowSizeDependentResources()
	{
#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		// Windows Phone does not support resizing the swap chain, so clear it instead of resizing.
		mSwapChain = nullptr;
#endif

		ID3D11RenderTargetView* nullViews[] = { nullptr };
		mDirect3DDeviceContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		mRenderTargetView = nullptr;
		mDepthStencilView = nullptr;
		mDirect3DDeviceContext->Flush();

		mGetRenderTargetSize(mRenderTargetSize);
		if (mSwapChain == nullptr)
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

			swapChainDesc.Width = mRenderTargetSize.cx;
			swapChainDesc.Height = mRenderTargetSize.cy;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
			swapChainDesc.SampleDesc.Count = mMultiSamplingCount;
			swapChainDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
#else
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
#endif

			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = DefaultBufferCount;
#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#else
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
#endif

			ComPtr<IDXGIDevice3> dxgiDevice;
			ThrowIfFailed(mDirect3DDevice.As(&dxgiDevice));

			ComPtr<IDXGIAdapter> dxgiAdapter;
			ThrowIfFailed(dxgiDevice->GetAdapter(&dxgiAdapter));

			ComPtr<IDXGIFactory2> dxgiFactory;
			ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

			void* window = mGetWindow();

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = { 0 };
			fullScreenDesc.RefreshRate.Numerator = mFrameRate;
			fullScreenDesc.RefreshRate.Denominator = 1;
			fullScreenDesc.Windowed = !mIsFullScreen;

			ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(mDirect3DDevice.Get(), reinterpret_cast<HWND>(window), &swapChainDesc, &fullScreenDesc, nullptr, &mSwapChain), "IDXGIDevice::CreateSwapChainForHwnd() failed.");
#else
			ThrowIfFailed(dxgiFactory->CreateSwapChainForCoreWindow(mDirect3DDevice.Get(), reinterpret_cast<IUnknown*>(window), &swapChainDesc, nullptr, &mSwapChain), "IDXGIFactory2::CreateSwapChainForCoreWindow() failed.");
#endif

			ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));
		}
		else
		{
			// Resize existing swap chain
			HRESULT hr = mSwapChain->ResizeBuffers(DefaultBufferCount, mRenderTargetSize.cx, mRenderTargetSize.cy, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			{
				// If the device was removed for any reason, a new device and swap chain will need to be created.
				HandleDeviceLost();

				// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
				// and correctly set up the new device.
				return;
			}
			else
			{
				ThrowIfFailed(hr, "IDXGISwapChain1::ResizeBuffers() failed.");
			}
		}

		// Create a render target view
		ComPtr<ID3D11Texture2D> backBuffer;
		ThrowIfFailed(mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)), "IDXGISwapChain1::GetBuffer() failed.");
		backBuffer->GetDesc(&mBackBufferDesc);
		ThrowIfFailed(mDirect3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, mRenderTargetView.GetAddressOf()), "IDXGIDevice::CreateRenderTargetView() failed.");

		// Create a depth-stencil view
		D3D11_TEXTURE2D_DESC depthStencilDesc = { 0 };
		depthStencilDesc.Width = mRenderTargetSize.cx;
		depthStencilDesc.Height = mRenderTargetSize.cy;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
		depthStencilDesc.SampleDesc.Count = mMultiSamplingCount;
		depthStencilDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
#else
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
#endif

		ComPtr<ID3D11Texture2D> depthStencilBuffer;
		ThrowIfFailed(mDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()), "IDXGIDevice::CreateTexture2D() failed.");

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);
#else
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
#endif
		ThrowIfFailed(mDirect3DDevice->CreateDepthStencilView(depthStencilBuffer.Get(), &depthStencilViewDesc, &mDepthStencilView), "IDXGIDevice::CreateDepthStencilView() failed.");

		// Set the viewport to the entire window
		mViewport = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(mRenderTargetSize.cx), static_cast<float>(mRenderTargetSize.cy));

		// Set render targets and viewport through render target stack	
		Begin();
	}

	void Game::HandleDeviceLost()
	{
		mSwapChain = nullptr;

		if (mDeviceNotify != nullptr)
		{
			mDeviceNotify->OnDeviceLost();
		}

		CreateDeviceResources();
		CreateWindowSizeDependentResources();

		if (mDeviceNotify != nullptr)
		{
			mDeviceNotify->OnDeviceRestored();
		}
	}
}