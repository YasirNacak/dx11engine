#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Entity.h"

namespace s3d { namespace entitysystem {

	class Manager
	{
	public:
		void Update(float mFT);
		void Draw();
		void Refresh();
		Entity& AddEntity(std::string name);
		std::vector<std::unique_ptr<Entity>> Entities;
	};
} }