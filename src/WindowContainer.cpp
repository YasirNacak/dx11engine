#include "WindowContainer.h"

namespace s3d {
	LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		// these two are only used in mouse events
		// they mean something completely different with other messages
		const int mousePosX = LOWORD(lParam);
		const int mousePosY = HIWORD(lParam);

		switch (uMessage)
		{
		case WM_KEYDOWN:
		{
			// https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-keydown
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
			// https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-keyup
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
		case WM_LBUTTONDOWN:
		{
			_mouse.OnLeftPressed(mousePosX, mousePosY);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			_mouse.OnLeftReleased(mousePosX, mousePosY);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			_mouse.OnRightPressed(mousePosX, mousePosY);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			_mouse.OnRightReleased(mousePosX, mousePosY);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			_mouse.OnMiddlePressed(mousePosX, mousePosY);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			_mouse.OnMiddleReleased(mousePosX, mousePosY);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const auto scrollDir = GET_WHEEL_DELTA_WPARAM(wParam);
			if(scrollDir > 0)
				_mouse.OnWheelUp(mousePosX, mousePosY);
			else if(scrollDir < 0)
				_mouse.OnWheelDown(mousePosX, mousePosY);
			break;
		}
		case WM_MOUSEMOVE:
		{
			_mouse.OnMouseMove(mousePosX, mousePosY);
			return 0;
		}
		default:
			return DefWindowProc(hwnd, uMessage, wParam, lParam);
		}
	}
}