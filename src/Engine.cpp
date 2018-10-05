#include "Engine.h"

namespace s3d {
	bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		if(!this->_renderWindow.Initialize(
			this,
			hInstance,
			windowTitle,
			windowClass,
			width,
			height))
		{
			return false;
		}
		if(_graphics.Initialize(this->_renderWindow.GetWindowHandle(), width, height))
		{
			return false;
		}

		return true;
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

		while(!_mouse.IsEventBufferEmpty())
		{
			auto me = _mouse.ReadEvent();
		}

		_graphics.RenderFrame();
	}
}