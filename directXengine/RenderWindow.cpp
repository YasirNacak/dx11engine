#include "RenderWindow.h"

bool RenderWindow::Initialize(
	HINSTANCE		hInstance,
	std::string		windowTitle,
	std::string		windowClass,
	int				width,
	int				height) {
	
	this->_hInstance = hInstance;
	this->_windowTitle = windowTitle;
	this->_windowTitleWide = StringConverter::StringToWide(this->_windowTitle);
	this->_windowClass = windowClass;
	this->_windowClassWide = StringConverter::StringToWide(this->_windowClass);
	this->_width = width;
	this->_height = height;

	this->RegisterWindowClass();
	
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-createwindowexa
	this->_handle = 
		CreateWindowEx(
			0,
			this->_windowClassWide.c_str(),
			this->_windowTitleWide.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			0,
			0,
			this->_width,
			this->_height,
			NULL,
			NULL,
			this->_hInstance,
			nullptr);

	if (!this->_handle) {
		ErrorLogger::Log(GetLastError(), "CreateWindow failed for " + this->_windowTitle);
		return false;
	}

	ShowWindow(this->_handle, SW_SHOW);
	SetForegroundWindow(this->_handle);
	SetFocus(this->_handle);

	return true;
}

bool RenderWindow::ProcessMessages() {
	MSG message;

	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366920(v=vs.85).aspx
	ZeroMemory(&message, sizeof(MSG));

	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-peekmessagea
	if (PeekMessage(
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

RenderWindow::~RenderWindow() {
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-unregisterclassa
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-destroywindow
	if (this->_handle) {
		UnregisterClass(this->_windowClassWide.c_str(), this->_hInstance);
		DestroyWindow(this->_handle);
	}
}

void RenderWindow::RegisterWindowClass() {
	// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-tagwndclassexa

	WNDCLASSEX windowClass;

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = this->_hInstance;
	windowClass.hIcon = NULL;
	windowClass.hIconSm = NULL;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = this->_windowClassWide.c_str();
	windowClass.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&windowClass);
}