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

		FullEntity(
				EntityID id, EntityManager& entity_manager, ComponentManager& component_manager
				, SystemManager& system_manager
		);

		template<typename ComponentType>
		ComponentType& get();

		template<typename ComponentType>
		const ComponentType& get() const;

		template<typename ComponentType, typename... Args>
		requires std::constructible_from<ComponentManager, Args...>
		void set(Args&&... args);

		template<typename ComponentType>
		void remove() noexcept;

		template<typename ComponentType>
		[[nodiscard]] bool has() const;

		[[nodiscard]] bool valid() const noexcept;

	private:
		EntityID id_;
		EntityManager& entity_manager_;
		ComponentManager& component_manager_;
		SystemManager& system_manager_;
	};

	FullEntity::FullEntity(
			EntityID id, EntityManager& entity_manager, ComponentManager& component_manager
			, SystemManager& system_manager
	)
	: id_(id), entity_manager_(entity_manager), component_manager_(component_manager), system_manager_(system_manager)
	{}

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
		assert(valid());
		assert(component_manager_.componentRegistered<ComponentType>());
		component_manager_.addComponent<ComponentType>(id_, std::forward<Args>(args)...);

		const auto component_type_id = component_manager_.getComponentTypeID<ComponentType>();
		auto mask = entity_manager_.getComponentMask(id_);
		if(!mask.test(component_type_id))
		{
			mask.set(component_type_id, true);
			entity_manager_.setComponentMask(id_, mask);
			system_manager_.updateEntityMask(id_, mask);
		}
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

	template<typename ComponentType>
	void FullEntity::remove() noexcept
	{
		component_manager_.removeComponent<ComponentType>(id_);

		const auto component_type_ID = component_manager_.getComponentTypeID<ComponentType>();

		auto mask = entity_manager_.getComponentMask(id_);
		mask.set(component_type_ID, false);
		entity_manager_.setComponentMask(id_, mask);
		system_manager_.updateEntityMask(id_, entity_manager_.getComponentMask(id_));
	}
}

#endif //MAGMATIC_FULLENTITY_HPP
