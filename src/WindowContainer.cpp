#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMessage, wParam, lParam);
}
