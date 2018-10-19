#pragma once
#include <string>
#include "utility/Timer.h"
#include "entitysystem/Manager.h"
#include "WindowContainer.h"

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
		void AddEntity(std::string name);

		void ShowDebugPanels();

	private:
		utility::Timer _timer;
		entitysystem::Manager _entityManager;
		int _windowWidth;
		int _windowHeight;
	};
}
