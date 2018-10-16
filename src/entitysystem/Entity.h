#pragma once
#include <vector>
#include <bitset>
#include <memory>
#include <array>
#include "Component.h"

namespace s3d { namespace entitysystem
{
	class Manager;

	constexpr std::size_t maxComponents{ 32 };
	
	using ComponentArray = std::array<Component*, maxComponents>;
	using ComponentBitset = std::bitset<maxComponents>;

	class Entity
	{
	public:
		Entity(Manager& manager);
		void Update(float mFT);
		void Draw();
		bool IsAlive() const;
		void Destroy();

	public:
		template <typename T>
		bool HasComponent() const
		{
			return _componentBitset[getComponentTypeID<T>()];
		}

		template <typename T, typename... TArgs>
		T& addComponent(TArgs&&... mArgs)
		{
			assert(!HasComponent<T>());

			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->Entity = this;
			std::unique_ptr<Component> uPtr{ c };
			_componentList.emplace_back(std::move(uPtr));

			_componentArray[getComponentTypeID<T>()] = c;
			_componentBitset[getComponentTypeID<T>()] = true;

			c->Init();
			return *c;
		}

		template <typename T>
		T& GetComponent() const
		{
			assert(HasComponent<T>());
			auto ptr(_componentArray[getComponentTypeID<T>()]);
			return *reinterpret_cast<T*>(ptr);
		}


	private:
		Manager& _manager;

		bool _isAlive = true;
		std::vector<std::unique_ptr<Component>> _componentList;
		ComponentArray _componentArray;
		ComponentBitset _componentBitset;
	};
} }