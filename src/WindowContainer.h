#pragma once
#include "RenderWindow.h"

class WindowContainer
{
public:
	LRESULT WindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
protected:
	RenderWindow _renderWindow;
};