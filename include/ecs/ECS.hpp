#ifndef MAGMATIC_ECS_HPP
#define MAGMATIC_ECS_HPP

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "EventRelay.hpp"


namespace magmatic::ecs
{
	class FullEntity;

	class ECS
	{
	public:
		using EntityID = EntityManager::EntityID;

		ECS();

		//Entity management

		[[nodiscard]] FullEntity createEntity();
		bool entityExists(EntityID id) const noexcept;
		void removeEntity(EntityID id);

		//Component management
		template<typename ComponentType>
		ComponentType& getComponent(EntityID id);

		template<typename ComponentType>
		const ComponentType& getComponent(EntityID id) const;

		template<typename ComponentType, typename... Args>
		requires std::constructible_from<ComponentType, Args...>
		void setComponent(EntityID id, Args&&... args);

		template<typename ComponentType>
		void removeComponent(EntityID id) noexcept;

		template<typename ComponentType>
		bool hasComponent(EntityID id) const;
	private:
		EventRelay event_relay_;

		EntityManager entity_manager_;
		ComponentManager component_manager_;
		SystemManager system_manager_;
	};

	template<typename ComponentType, typename... Args>
	requires std::constructible_from<ComponentType, Args...>
	void ECS::setComponent(ECS::EntityID id, Args&& ... args)
	{
		assert(component_manager_.componentRegistered<ComponentType>());
		component_manager_.addComponent<ComponentType>(id, std::forward<Args>(args)...);

		const auto component_type_id = component_manager_.getComponentTypeID<ComponentType>();
		auto mask = entity_manager_.getComponentMask(id);

		if(!mask.test(component_type_id))
		{
			mask.set(component_type_id, true);
			entity_manager_.setComponentMask(id, mask);
			system_manager_.updateEntityMask(id, mask);

			spdlog::debug("Magmatic: Created new component for entity");
		}
	}

	template<typename ComponentType>
	void magmatic::ecs::ECS::removeComponent(magmatic::ecs::ECS::EntityID id) noexcept
	{
		component_manager_.removeComponent<ComponentType>(id);

		const auto component_type_ID = component_manager_.getComponentTypeID<ComponentType>();
		auto mask = entity_manager_.getComponentMask(id);

		#if !defined(NDEBUG)
		if(!mask.test(component_type_ID))
		{
			spdlog::warn("Magmatic: Tried to remove component not registered for entity. ComponentType: {}", typeid(ComponentType).name());
		}
		#endif

		mask.set(component_type_ID, false);
		entity_manager_.setComponentMask(id, mask);
		system_manager_.updateEntityMask(id, mask);
	}

	bool ECS::entityExists(ECS::EntityID id) const noexcept
	{
		return entity_manager_.entityExists(id);
	}

	template<typename ComponentType>
	bool ECS::hasComponent(EntityID id) const
	{
		return component_manager_.hasComponent<ComponentType>(id);
	}

	template<typename ComponentType>
	ComponentType& ECS::getComponent(ECS::EntityID id)
	{
		assert(component_manager_.hasComponent<ComponentType>(id));
		return component_manager_.getComponent<ComponentType>(id);
	}

	template<typename ComponentType>
	const ComponentType& ECS::getComponent(EntityID id) const
	{
		assert(component_manager_.hasComponent<ComponentType>(id));
		return component_manager_.getComponent<ComponentType>(id);
	}
}

#endif //MAGMATIC_ECS_HPP
