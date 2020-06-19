#ifndef MAGMATIC_COMPONENTMAPPING_HPP
#define MAGMATIC_COMPONENTMAPPING_HPP

#include "ecs/EntityManager.hpp"
#include <unordered_map>
#include <map>
#include <spdlog/spdlog.h>


namespace magmatic::ecs
{
	class BaseComponentMapping
	{
	public:
		using EntityID = EntityManager::EntityID;
		virtual ~BaseComponentMapping() = default;

		virtual void remove(EntityID id) = 0;
	};

	template<typename T>
	class ComponentMapping : public BaseComponentMapping
	{
	public:
		using Type = T;
		using ComponentID = std::size_t;

		ComponentMapping();

		void insert(EntityID id, const T& component);
		void remove(EntityID id) override;

		T& get(EntityID id);
		const T& get(EntityID id) const;

		[[nodiscard]] bool exist(EntityID) const noexcept;

	private:
		std::vector<T> components;
		std::unordered_map<EntityID, ComponentID> entityToComponent;
		std::unordered_map<ComponentID, EntityID> componentToEntity;
		std::size_t size = 0;
	};

	template<typename T>
	ComponentMapping<T>::ComponentMapping()
	{
		components.resize(MAX_ENTITIES_COUNT);
	}

	template<typename T>
	void ComponentMapping<T>::insert(ComponentMapping::EntityID id, const T& component)
	{
		assert(size < MAX_ENTITIES_COUNT && "Component limit exceeded");
		if(entityToComponent.contains(id))
		{
			spdlog::error("Magmatic: Failed to add Component. Already exists");
			throw std::runtime_error("Component already exist");
		}

		components[size] = component;
		entityToComponent[id] = size;
		componentToEntity[size] = id;
		++size;
	}

	template<typename T>
	void ComponentMapping<T>::remove(ComponentMapping::EntityID id)
	{
		if(!exist(id))
		{
			spdlog::warn("Magmatic: Failed to remove component od entity. Not exist");
			return;
		}

		const ComponentID old_index = entityToComponent[id];
		std::swap(components[old_index], components[size-1]);

		const EntityID end_component_id = componentToEntity[size-1];
		entityToComponent[end_component_id] = old_index;
		componentToEntity[old_index] = end_component_id;

		entityToComponent.erase(id);
		componentToEntity.erase(size-1);
		components.pop_back();
	}

	template<typename T>
	T &ComponentMapping<T>::get(ComponentMapping::EntityID id)
	{
		try
		{
			const ComponentID componentID =  entityToComponent.at(id);
			return components[componentID];
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
			const ComponentID componentID =  entityToComponent.at(id);
			return components[componentID];
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
		return entityToComponent.contains(id);
	}
}

#endif //MAGMATIC_COMPONENTMAPPING_HPP
