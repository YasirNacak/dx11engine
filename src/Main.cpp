#include <Windows.h>
#include "Engine.h"

int APIENTRY wWinMain(
	_In_ HINSTANCE		hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_ LPWSTR			lpCmdLine,
	_In_ int			nCmdShow) {

	s3d::Engine engine;

	if(engine.Initialize(hInstance, "dx11Engine", "dx11EngineWinClass", 800, 600))
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