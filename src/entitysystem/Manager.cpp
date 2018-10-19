#include "Manager.h"

namespace s3d{ namespace entitysystem
{
	void Manager::Update(float mFT)
	{
		for (auto& e : Entities) e->Update(mFT);
	}

	void Manager::Draw()
	{
		for (auto& e : Entities) e->Draw();
	}

	void Manager::Refresh()
	{
		Entities.erase(
			std::remove_if(std::begin(Entities), std::end(Entities),
				[](const std::unique_ptr<Entity>& entity)
		{
			return !entity->IsAlive();
		}),
			std::end(Entities));
	}

	Entity& Manager::AddEntity(std::string name)
	{
		auto e(new Entity(name, *this));
		std::unique_ptr<Entity> uPtr{ e };
		Entities.emplace_back(std::move(uPtr));
		return *e;
	}
} }