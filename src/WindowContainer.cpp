#include "WindowContainer.h"

namespace s3d {
	LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		switch (uMessage)
		{
		case WM_KEYDOWN:
		{
			const auto keycode = static_cast<unsigned char>(wParam);
			if(_keyboard.IsKeysAutoRepeat())
			{
				_keyboard.OnKeyPressed(keycode);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if (!wasPressed)
				{
					_keyboard.OnKeyPressed(keycode);
				}
			}
			return 0;
		}
		case WM_KEYUP:
		{
			const auto keycode = static_cast<unsigned char>(wParam);
			_keyboard.OnKeyReleased(keycode);
			return 0;
		}
		case WM_CHAR:
		{
			// https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-char
			const auto ch = static_cast<unsigned char>(wParam);
			if(_keyboard.IsCharsAutoRepeat())
			{
				_keyboard.OnChar(ch);
			}
			else
			{
				const bool wasPressed = lParam & 0x40000000;
				if(!wasPressed)
				{
					_keyboard.OnChar(ch);
				}
			}
			return 0;
		}
		default:
			return DefWindowProc(hwnd, uMessage, wParam, lParam);
		}
	}
}