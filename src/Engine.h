#pragma once
#include "WindowContainer.h"
class Engine : WindowContainer
{
public:
	bool Initialize(
		HINSTANCE		hInstance,
		std::string		windowTitle,
		std::string		windowClass,
		int				width = 320,
		int				height = 240);
	bool ProcessMessages();
};

