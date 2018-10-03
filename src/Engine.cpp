#include "Engine.h"

namespace s3d {
	bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		return this->_renderWindow.Initialize(this, hInstance, windowTitle, windowClass, width, height);
	}

	bool Engine::ProcessMessages()
	{
		return this->_renderWindow.ProcessMessages();
	}

	void Engine::Update()
	{
		while(!_keyboard.IsCharBufferEmpty())
		{
			const auto ch = _keyboard.ReadChar();
		}
		while(!_keyboard.IsKeyBufferEmpty())
		{
			auto kbe = _keyboard.ReadKey();
			const auto keycode = kbe.GetKeyCode();
		}
	}
}