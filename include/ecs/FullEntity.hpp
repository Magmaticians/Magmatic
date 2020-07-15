#ifndef MAGMATIC_FULLENTITY_HPP
#define MAGMATIC_FULLENTITY_HPP

#include "EntityManager.hpp"
#include "ComponentManager.hpp"

namespace magmatic::ecs
{
	class FullEntity
	{
	public:
		using EntityID = EntityManager::EntityID;

		FullEntity(EntityID id, EntityManager& entity_manager, ComponentManager& component_manager);

		template<typename ComponentType>
		ComponentType& get();

		template<typename ComponentType>
		const ComponentType& get() const;

		template<typename ComponentType, typename... Args>
		requires std::constructible_from<ComponentManager, Args...>
		void set(Args&&... args);

		template<typename ComponentType>
		void set(ComponentType&& component);

		template<typename ComponentType>
		[[nodiscard]] bool has() const;

		[[nodiscard]] bool valid() const noexcept;

	private:
		EntityID id_;
		EntityManager& entity_manager_;
		ComponentManager& component_manager_;
	};

	FullEntity::FullEntity(EntityID id, EntityManager& entity_manager, ComponentManager& component_manager)
	: id_(id), entity_manager_(entity_manager), component_manager_(component_manager) {}

	template<typename ComponentType>
	ComponentType& FullEntity::get()
	{
		assert(valid());
		assert(component_manager_.hasComponent<ComponentType>(id_));
		return component_manager_.getComponent<ComponentType>(id_);
	}

	template<typename ComponentType>
	const ComponentType &FullEntity::get() const
	{
		assert(valid());
		assert(component_manager_.hasComponent<ComponentType>(id_));
		return component_manager_.getComponent<ComponentType>(id_);
	}

	template<typename ComponentType, typename... Args>
	requires std::constructible_from<ComponentManager, Args...>
	void FullEntity::set(Args &&... args)
	{
		component_manager_.addComponent<ComponentType>(std::forward<Args>(args)...);
	}

	template<typename ComponentType>
	void FullEntity::set(ComponentType&& component)
	{
		component_manager_.addComponent<ComponentType>(std::forward(component));
	}

	template<typename ComponentType>
	bool FullEntity::has() const
	{
		assert(valid());
		return component_manager_.hasComponent<ComponentType>(id_);
	}

	bool FullEntity::valid() const noexcept
	{
		return entity_manager_.entityExists(id_);
	}
}

#endif //MAGMATIC_FULLENTITY_HPP
