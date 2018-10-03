#include <Windows.h>
#include "RenderWindow.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY wWinMain(
	_In_ HINSTANCE		hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_ LPWSTR			lpCmdLine,
	_In_ int			nCmdShow) {

	RenderWindow renderWindow;
	renderWindow.Initialize(hInstance, "dx11Engine", "dx11EngineWinClass", 1280, 720);
	while (renderWindow.ProcessMessages()) {

	}
	return 0;
}