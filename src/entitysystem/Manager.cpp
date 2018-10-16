#include "Manager.h"

namespace s3d{ namespace entitysystem
{
	void Manager::Update(float mFT)
	{
		for (auto& e : _entities) e->Update(mFT);
	}

	void Manager::Draw()
	{
		for (auto& e : _entities) e->Draw();
	}

	void Manager::Refresh()
	{
		_entities.erase(
			std::remove_if(std::begin(_entities), std::end(_entities),
				[](const std::unique_ptr<Entity>& entity)
		{
			return !entity->IsAlive();
		}),
			std::end(_entities));
	}

	Entity & Manager::AddEntity()
	{
		auto e(new Entity(*this));
		std::unique_ptr<Entity> uPtr{ e };
		_entities.emplace_back(std::move(uPtr));
		return *e;
	}
} }