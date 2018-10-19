#include "Entity.h"

namespace s3d { namespace entitysystem {
	Entity::Entity(std::string name, Manager& manager) :
		Name(name), _manager(manager)
	{
	}

	void Entity::Update(float mFT)
	{
		for (auto& c : ComponentList)
			c->Update(mFT);
	}

	void Entity::Draw()
	{
		for (auto& c : ComponentList)
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