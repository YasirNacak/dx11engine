#pragma once
#include <vector>
#include <memory>
#include "Entity.h"

namespace s3d { namespace entitysystem {
	using Group = std::size_t;

	class Manager
	{
	public:
		void Update(float mFT);
		void Draw();
		void AddToGroup(Entity* entity, Group group);
		std::vector<Entity*>& GetEntitiesByGroup(Group group);
		void Refresh();
		Entity& AddEntity();

	private:
		std::vector<std::unique_ptr<Entity>> _entities;
	};
} }