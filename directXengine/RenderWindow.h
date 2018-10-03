#pragma once
#include "ErrorLogger.h"

class RenderWindow {
public:
	bool Initialize(HINSTANCE		hInstance,
					std::string		windowTitle,
					std::string		windowClass,
					int				width = 320,
					int				height = 240);
	bool ProcessMessages();
	~RenderWindow();

private:
	void RegisterWindowClass();

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