#pragma once
#include "WindowContainer.h"
#include "utility/Timer.h"

namespace s3d {
	class Engine : WindowContainer
	{
	public:
		bool Initialize(
			HINSTANCE		hInstance,
			std::string		windowTitle,
			std::string		windowClass,
			int				width = 320,
			int				height = 240);
		bool ProcessMessages();
		void Update();

	private:
		utility::Timer _timer;
	};
}