#include "pch.h"

using namespace Library;
using namespace Rendering;
using namespace std;

void Shutdown(const wstring& className);

const SIZE RenderTargetSize = { 1024, 768 };
HWND mWindowHandle;
WNDCLASSEX mWindow;
unique_ptr<RenderingGame> mGame;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);

	SetCurrentDirectory(UtilityWin32::ExecutableDirectory().c_str());

	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED), "Error initializing COM.");

	static const wstring windowClassName = L"RenderingClass";
	static const wstring windowTitle = L"DirectX Essentials";

	UtilityWin32::InitializeWindow(mWindow, mWindowHandle, instance, windowClassName, windowTitle, RenderTargetSize, showCommand);

	auto getRenderTargetSize = [](SIZE& renderTargetSize)
	{
		renderTargetSize = RenderTargetSize;
	};

	auto getWindow = [&]() -> void*
	{
		return reinterpret_cast<void*>(mWindowHandle);
	};

	mGame = make_unique<RenderingGame>(getWindow, getRenderTargetSize);
	mGame->UpdateRenderTargetSize();
	mGame->Initialize();
	
	MSG message = { 0 };

	try
	{
		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				mGame->Run();
			}
		}
	}
	catch (GameException ex)
	{
		MessageBox(mWindowHandle, ex.whatw().c_str(), windowTitle.c_str(), MB_ABORTRETRYIGNORE);
	}

	Shutdown(windowClassName);

	CoUninitialize();

	return static_cast<int>(message.wParam);
}

void Shutdown(const wstring& className)
{
	//mGame->Shutdown();
	UnregisterClass(className.c_str(), mWindow.hInstance);
}