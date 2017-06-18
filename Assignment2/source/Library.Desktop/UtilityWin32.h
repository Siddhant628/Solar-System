#pragma once

#include <string>

namespace Library
{
	class UtilityWin32
	{
	public:
		static void InitializeWindow(WNDCLASSEX& window, HWND& windowHandle, HINSTANCE instance, const std::wstring& className, const std::wstring windowTitle, const SIZE& renderTargetSize, int showCommand);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static POINT CenterWindow(const SIZE& windowSize);

		static std::string CurrentDirectory();
		static std::wstring ExecutableDirectory();
		static void PathJoin(std::wstring& dest, const std::wstring& sourceDirectory, const std::wstring& sourceFile);
		static void GetPathExtension(const std::wstring& source, std::wstring& dest);

		UtilityWin32() = delete;
		UtilityWin32(const UtilityWin32&) = delete;
		UtilityWin32& operator=(const UtilityWin32&) = delete;
		UtilityWin32(UtilityWin32&&) = delete;
		UtilityWin32& operator=(UtilityWin32&&) = delete;
		~UtilityWin32() = default;
	};
}