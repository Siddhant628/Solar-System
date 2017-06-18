#include "pch.h"
#include "UtilityWin32.h"
#include <windows.h>
#include <string>
#include <Shlwapi.h>

using namespace std;

namespace Library
{
	void UtilityWin32::InitializeWindow(WNDCLASSEX& window, HWND& windowHandle, HINSTANCE instance, const wstring& className, const wstring windowTitle, const SIZE& renderTargetSize, int showCommand)
	{
		ZeroMemory(&window, sizeof(window));
		window.cbSize = sizeof(WNDCLASSEX);
		window.style = CS_CLASSDC;
		window.lpfnWndProc = WndProc;
		window.hInstance = instance;
		window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		window.lpszClassName = className.c_str();

		RECT windowRectangle = { 0, 0, renderTargetSize.cx, renderTargetSize.cy };
		AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&window);
		POINT center = CenterWindow(renderTargetSize);
		windowHandle = CreateWindow(className.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, instance, nullptr);

		ShowWindow(windowHandle, showCommand);
		UpdateWindow(windowHandle);
	}

	POINT UtilityWin32::CenterWindow(const SIZE& windowSize)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		POINT center;
		center.x = (screenWidth - windowSize.cx) / 2;
		center.y = (screenHeight - windowSize.cy) / 2;

		return center;
	}

	LRESULT WINAPI UtilityWin32::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			break;

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	string UtilityWin32::CurrentDirectory()
	{
		WCHAR buffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, buffer);
		wstring currentDirectoryW(buffer);

		return string(currentDirectoryW.begin(), currentDirectoryW.end());
	}

	wstring UtilityWin32::ExecutableDirectory()
	{
		WCHAR buffer[MAX_PATH];
		GetModuleFileName(nullptr, buffer, MAX_PATH);
		PathRemoveFileSpec(buffer);

		return wstring(buffer);
	}

	void UtilityWin32::PathJoin(wstring& dest, const wstring& sourceDirectory, const wstring& sourceFile)
	{
		WCHAR buffer[MAX_PATH];

		PathCombine(buffer, sourceDirectory.c_str(), sourceFile.c_str());
		dest = buffer;
	}

	void UtilityWin32::GetPathExtension(const wstring& source, wstring& dest)
	{
		dest = PathFindExtension(source.c_str());
	}
}