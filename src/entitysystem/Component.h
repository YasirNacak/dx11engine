#pragma once

namespace s3d { namespace entitysystem
{
	class Entity;

	using ComponentID = std::size_t;

	struct Component
	{
		Entity* Entity;

		virtual void Init() {}
		virtual void Update(float mFT) {}
		virtual void Draw() {}

		virtual ~Component() {}
	};

	namespace internal
	{
		inline ComponentID getUniqueComponentID() noexcept
		{
			static ComponentID lastID{ 0u };
			return lastID++;
		}
	}

	template <typename T>
	ComponentID getComponentTypeID() noexcept
	{
		static ComponentID typeID{ internal::getUniqueComponentID() };
		return typeID;
	}
} }