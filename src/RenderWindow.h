#pragma once
#include "utility/ErrorLogger.h"

namespace s3d {
	class WindowContainer;

	class RenderWindow {
	public:
		bool Initialize(
			WindowContainer	*pWindowContainer,
			HINSTANCE		hInstance,
			std::string		windowTitle,
			std::string		windowClass,
			int				width = 320,
			int				height = 240);
		bool ProcessMessages();
		HWND GetWindowHandle() const;
		~RenderWindow();

	private:
		void RegisterWindowClass() const;

	private:
		HWND			_handle = NULL;
		HINSTANCE		_hInstance = NULL;
		std::string		_windowTitle = "";
		std::wstring	_windowTitleWide = L"";
		std::string		_windowClass = "";
		std::wstring	_windowClassWide = L"";
		int				_width = 320;
		int				_height = 240;
	};
}
