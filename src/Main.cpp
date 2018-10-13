#include <Windows.h>
#include "Engine.h"

int APIENTRY wWinMain(
	_In_ HINSTANCE		hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_ LPWSTR			lpCmdLine,
	_In_ int			nCmdShow) {

	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr))
	{
		s3d::utility::ErrorLogger::Log("Failed to call CoInitialize.");
		return -1;
	}

	s3d::Engine engine;
	if(engine.Initialize(hInstance, "dx11Engine", "dx11EngineWinClass", 1920, 1080))
	{
		while (engine.ProcessMessages()) {
			engine.Update();
		}
	}
	else
	{
		s3d::utility::ErrorLogger::Log("Failed to initialize engine");
		return -1;
	}

	return 0;
}