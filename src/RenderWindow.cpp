#include "WindowContainer.h"

namespace s3d {
	bool RenderWindow::Initialize(
		WindowContainer	*pWindowContainer,
		HINSTANCE		hInstance,
		std::string		windowTitle,
		std::string		windowClass,
		int				width,
		int				height) {

		this->_hInstance = hInstance;
		this->_windowTitle = windowTitle;
		this->_windowTitleWide = utility::StringConverter::StringToWide(this->_windowTitle);
		this->_windowClass = windowClass;
		this->_windowClassWide = utility::StringConverter::StringToWide(this->_windowClass);
		this->_width = width;
		this->_height = height;

		this->RegisterWindowClass();

		const int screenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2 - this->_width / 2;
		const int screenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2 - this->_height / 2;

		RECT windowRect;
		windowRect.left = screenCenterX;
		windowRect.top = screenCenterY;
		windowRect.right = windowRect.left + this->_width;
		windowRect.bottom = windowRect.top + this->_height;

		AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		this->_handle =
			CreateWindowEx(
				0,
				this->_windowClassWide.c_str(),
				this->_windowTitleWide.c_str(),
				WS_POPUPWINDOW | WS_CAPTION,
				windowRect.left,
				windowRect.top,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				NULL,
				NULL,
				this->_hInstance,
				pWindowContainer);

		if (!this->_handle) {
			utility::ErrorLogger::Log(GetLastError(), "CreateWindow failed for " + this->_windowTitle);
			return false;
		}

		ShowWindow(this->_handle, SW_SHOW);
		SetForegroundWindow(this->_handle);
		SetFocus(this->_handle);

		return true;
	}

	bool RenderWindow::ProcessMessages() {
		MSG message;

		ZeroMemory(&message, sizeof(MSG));

		while (PeekMessage(
			&message,
			this->_handle,
			0,
			0,
			PM_REMOVE)) {

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		if (message.message == WM_NULL) {
			if (!IsWindow(this->_handle)) {
				this->_handle = NULL;
				UnregisterClass(this->_windowClassWide.c_str(), this->_hInstance);
				return false;
			}
		}

		return true;
	}

	HWND RenderWindow::GetWindowHandle() const
	{
		return this->_handle;
	}

	RenderWindow::~RenderWindow() {
		if (this->_handle) {
			UnregisterClass(this->_windowClassWide.c_str(), this->_hInstance);
			DestroyWindow(this->_handle);
		}
	}

	LRESULT CALLBACK HandleMessageRedirect(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
		switch (uMessage)
		{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		default:
		{
			WindowContainer * const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			return pWindow->WindowProc(hwnd, uMessage, wParam, lParam);
		}
		}
	}

	LRESULT CALLBACK SetupMessageHandler(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
		switch (uMessage)
		{
		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			auto *pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
			if (pWindow == nullptr) {
				utility::ErrorLogger::Log("Pointer to window container is null during WM_NCCREATE.");
				exit(-1);
			}
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));
			return pWindow->WindowProc(hwnd, uMessage, wParam, lParam);
		}
		default:
		{
			return DefWindowProc(hwnd, uMessage, wParam, lParam);
		}
		}
	}

	void RenderWindow::RegisterWindowClass() const
	{
		WNDCLASSEX windowClass;

		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = SetupMessageHandler;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = this->_hInstance;
		windowClass.hIcon = NULL;
		windowClass.hIconSm = NULL;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = this->_windowClassWide.c_str();
		windowClass.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&windowClass);
	}
}