#pragma once
#include "RenderWindow.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "graphics/Graphics.h"

namespace s3d {

	class WindowContainer
	{
	public:
		WindowContainer();
		LRESULT WindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	protected:
		RenderWindow _renderWindow;
		input::Keyboard _keyboard;
		input::Mouse _mouse;
		graphics::Graphics _graphics;
	};
}