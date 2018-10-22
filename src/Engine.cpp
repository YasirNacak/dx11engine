#include "Engine.h"
#include "entitysystem/components/TransformComponent.h"

namespace s3d {
	bool Engine::Initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClass, int width, int height)
	{
		_keyboard.DisableAutoRepeatChars();
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);

		_windowWidth = width;
		_windowHeight = height;

		_timer.Start();

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
		float dt = static_cast<float>(_timer.GetMillisecondsElapsed());
		_timer.Restart();

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

		const float cameraSpeed = 0.005f;

		XMVECTOR moveSpeedVector{cameraSpeed, cameraSpeed, cameraSpeed};
		moveSpeedVector = DirectX::XMVectorMultiply(moveSpeedVector, {dt, dt, dt});

		if(this->_keyboard.IsKeyPressed('W'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetForwardVector(), moveSpeedVector));
		}

		if (this->_keyboard.IsKeyPressed('S'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetBackwardVector(), moveSpeedVector));
		}

		if (this->_keyboard.IsKeyPressed('A'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetLeftVector(), moveSpeedVector));
		}

		if (this->_keyboard.IsKeyPressed('D'))
		{
			this->_graphics.MainCamera.AdjustPosition(DirectX::XMVectorMultiply(this->_graphics.MainCamera.GetRightVector(), moveSpeedVector));
		}

		if (_keyboard.IsKeyPressed('X'))
		{
			this->_graphics.MainCamera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
		}

		if (_keyboard.IsKeyPressed('Z'))
		{
			this->_graphics.MainCamera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
		}

		_graphics.RenderFrame(*this);
	}

	void Engine::AddEntity(std::string name)
	{
		_entityManager.AddEntity(name);
	}

	void Engine::ShowDebugPanels()
	{
		static bool shouldShowEntityEditor = false;
		float mainMenuPadding;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		if(ImGui::BeginMainMenuBar())
		{
			mainMenuPadding = ImGui::GetWindowSize().y;
			if(ImGui::BeginMenu("View"))
			{
				if(ImGui::MenuItem("Entity Editor"))
				{
					shouldShowEntityEditor = !shouldShowEntityEditor;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if(shouldShowEntityEditor)
		{
			ImGui::Begin("Entity Editor", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::SetWindowPos({ 0, mainMenuPadding});
			ImGui::SetWindowSize(
				{ static_cast<float>(_windowWidth / 5), static_cast<float>(_windowHeight - mainMenuPadding) });
			if (ImGui::Button("Add Entity", { static_cast<float>(_windowWidth / 5 - ImGui::GetStyle().WindowPadding.x * 2), 0 }))
			{
				static int entityCount = 0;
				auto& ent = _entityManager.AddEntity("entity" + std::to_string(entityCount));
				auto comp = ent.AddComponent<entitysystem::components::TransformComponent>();
				comp.Init();
				entityCount++;
			}
			int i = 0;
			for (const auto& e : _entityManager.Entities)
			{
				if (ImGui::CollapsingHeader(e.get()->Name.c_str()))
				{
					char entityName[32] = "";
					strcpy_s(entityName, e.get()->Name.c_str());
					ImGui::BulletText("Name");
					char inputTextLabel[32] = "##EntNameLabel";
					strcat_s(inputTextLabel, std::to_string(i).c_str());
					if (ImGui::InputText(inputTextLabel, entityName, 16, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						e.get()->Name = entityName;
					}
					ImGui::BulletText("Components");
					for (const auto& c : e.get()->ComponentList)
					{
						std::string nodeLabel = c.get()->Name + "##" + e.get()->Name;
						if(ImGui::TreeNode(nodeLabel.c_str()))
						{
							ImGui::TreePop();
						}
					}
				}

				i++;
			}
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
