#include "Entity.h"

namespace s3d { namespace entitysystem {
	Entity::Entity(Manager& manager) :
		_manager(manager)
	{
	}

	void Entity::Update(float mFT)
	{
		for (auto& c : _componentList)
			c->Update(mFT);
	}

	void Entity::Draw()
	{
		for (auto& c : _componentList)
			c->Draw();
	}

	bool Entity::IsAlive() const
	{
		return _isAlive;
	}

	void Entity::Destroy()
	{
		_isAlive = false;
	}
} }