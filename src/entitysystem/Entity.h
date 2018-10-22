#pragma once
#include <vector>
#include <bitset>
#include <memory>
#include <array>
#include <string>
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
		Entity(std::string name, Manager& manager);
		void Update(float mFT);
		void Draw();
		bool IsAlive() const;
		void Destroy();
		std::string Name;
		std::vector<std::unique_ptr<Component>> ComponentList;

	public:
		template <typename T>
		bool HasComponent() const
		{
			return _componentBitset[getComponentTypeID<T>()];
		}

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs)
		{
			//assert(!HasComponent<T>());

			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->Entity = this;
			std::unique_ptr<Component> uPtr{ c };
			ComponentList.emplace_back(std::move(uPtr));

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
		ComponentArray _componentArray;
		ComponentBitset _componentBitset;
	};
} }