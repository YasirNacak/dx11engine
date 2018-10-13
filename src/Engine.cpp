#include "Engine.h"

namespace s3d {
	bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		/*
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
		 */

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

		if(!_graphics.Initialize(this->_renderWindow.GetWindowHandle(), width, height))
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

		/*
		while (!_mouse.IsEventBufferEmpty())
		{
			auto me = _mouse.ReadEvent();
		}
		*/

		// todo(yasir): move the following logic to a component 
		// todo(yasir): attached to the camera when entity system is done

		while(!_mouse.IsEventBufferEmpty())
		{
			auto me = _mouse.ReadEvent();
			if(_mouse.IsMiddleDown())
			{
				if (me.GetType() == input::MouseEvent::RawMove)
				{
					this->_graphics.MainCamera.AdjustRotation(
						static_cast<float>(me.GetPosition().y) * 0.01f,
						static_cast<float>(me.GetPosition().x) * 0.01f, 0);
				}
			}	
		}

		const float cameraSpeed = 0.02f;

		if(this->_keyboard.IsKeyPressed('W'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetForwardVector(), { cameraSpeed, cameraSpeed, cameraSpeed }));
		}

		if (this->_keyboard.IsKeyPressed('S'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetBackwardVector(), { cameraSpeed, cameraSpeed, cameraSpeed }));
		}

		if (this->_keyboard.IsKeyPressed('A'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetLeftVector(), { cameraSpeed, cameraSpeed, cameraSpeed }));
		}

		if (this->_keyboard.IsKeyPressed('D'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetRightVector(), { cameraSpeed, cameraSpeed, cameraSpeed }));
		}

		if (_keyboard.IsKeyPressed('X'))
		{
			this->_graphics.MainCamera.AdjustPosition(0.0f, cameraSpeed, 0.0f);
		}

		if (_keyboard.IsKeyPressed('Z'))
		{
			this->_graphics.MainCamera.AdjustPosition(0.0f, -cameraSpeed, 0.0f);
		}

		_graphics.RenderFrame();
	}
}