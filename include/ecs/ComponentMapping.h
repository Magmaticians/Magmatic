#ifndef MAGMATIC_COMPONENTMAPPING_H
#define MAGMATIC_COMPONENTMAPPING_H

#include "ecs/EntityManager.h"
#include <unordered_map>
#include <map>
#include <spdlog/spdlog.h>


namespace magmatic::ecs
{
	template<typename T>
	class ComponentMapping
	{
	public:
		using EntityID = typename EntityManager::EntityID;
		void insert(EntityID id, const T& component);
		void remove(EntityID id);

		T& get(EntityID id);
		const T& get(EntityID id) const;

		[[nodiscard]] bool exist(EntityID) const noexcept;

	private:
		std::unordered_map<EntityID, T> components;
	};

	template<typename T>
	void ComponentMapping<T>::insert(ComponentMapping::EntityID id, const T& component)
	{
		if(components.contains(id))
		{
			spdlog::error("Magmatic: Cannot add more than one component of type: {}", typeid(T).name());
			throw std::runtime_error("Failed to add component to entity");
		}
		components.insert({id, component});
	}

	template<typename T>
	void ComponentMapping<T>::remove(ComponentMapping::EntityID id)
	{

		auto removed = components.erase(id);
		if(removed == 0)
		{
			spdlog::debug("Failed to remove component od entity. Not exist");
		}
	}

	template<typename T>
	T &ComponentMapping<T>::get(ComponentMapping::EntityID id)
	{
		try
		{
			return components.at(id);
		}
		catch (const std::out_of_range&)
		{
			spdlog::error("Magmatic: Failed to get component of entity. Not exist");
			throw;
		}
	}

	template<typename T>
	const T &ComponentMapping<T>::get(ComponentMapping::EntityID id) const
	{
		try
		{
			return components.at(id);
		}
		catch (const std::out_of_range&)
		{
			spdlog::error("Magmatic: Failed to get component of entity. Not exist");
			throw;
		}
	}

	template<typename T>
	bool ComponentMapping<T>::exist(ComponentMapping::EntityID id) const noexcept
	{
		return components.contains(id);
	}
}

#endif //MAGMATIC_COMPONENTMAPPING_H
