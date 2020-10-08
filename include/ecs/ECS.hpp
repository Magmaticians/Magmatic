#ifndef MAGMATIC_ECS_HPP
#define MAGMATIC_ECS_HPP

#include <utils/thread_pool/ThreadPool.hpp>
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "EventRelay.hpp"
#include "ECSTypes.hpp"


namespace magmatic::ecs
{
	class FullEntity;

	class ECS
	{
	public:

		explicit ECS(thread_pool_ptr thread_pool=std::make_shared<thread_pool_t>());

		void update(const delta_time_t& delta);
		EventRelay& eventRelay() noexcept;
		//Entity management

		[[nodiscard]] entity_id_t createEntity();
		bool entityExists(entity_id_t id) const noexcept;
		void removeEntity(entity_id_t id);

		//Component management
		template<typename ComponentType>
		void registerComponent();

		template<typename ComponentType>
		ComponentType& getComponent(entity_id_t id);

		template<typename ComponentType>
		const ComponentType& getComponent(entity_id_t id) const;

		template<typename ComponentType, typename... Args>
		requires std::constructible_from<ComponentType, Args...>
		void addComponent(entity_id_t id, Args&&... args);

		template<typename ComponentType>
		void removeComponent(entity_id_t id) noexcept;

		template<typename ComponentType>
		bool hasComponent(entity_id_t id) const;

		//system management
		template<typename T, typename... Args>
		requires std::constructible_from<T, Args...>
		auto registerSystem(Args&&... args);

	private:
		EventRelay event_relay_;

		EntityManager entity_manager_;
		ComponentManager component_manager_;
		SystemManager system_manager_;

		template<typename ComponentType>
		void updateEntitySignature(entity_id_t id);
	};

	template<typename ComponentType>
	void ECS::registerComponent()
	{
		component_manager_.registerComponent<ComponentType>();
	}

	template<typename ComponentType, typename... Args>
	requires std::constructible_from<ComponentType, Args...>
	void ECS::addComponent(entity_id_t id, Args&& ... args)
	{
		component_manager_.addComponent<ComponentType>(id, std::forward<Args>(args)...);

		updateEntitySignature<ComponentType>(id);
	}

	template<typename ComponentType>
	void magmatic::ecs::ECS::removeComponent(magmatic::ecs::entity_id_t id) noexcept
	{
		component_manager_.removeComponent<ComponentType>(id);

		const auto component_type_id = component_manager_.getComponentTypeId<ComponentType>();
		auto mask = entity_manager_.getComponentMask(id);

		#if !defined(NDEBUG)
		if(!mask.test(component_type_id))
		{
			spdlog::warn("Magmatic: Tried to remove component not registered for entity. ComponentType: {}", typeid(ComponentType).name());
		}
		#endif

		mask.set(component_type_id, false);
		entity_manager_.setComponentMask(id, mask);
		system_manager_.updateEntityMask(id, mask);
	}

	template<typename ComponentType>
	bool ECS::hasComponent(entity_id_t id) const
	{
		return component_manager_.hasComponent<ComponentType>(id);
	}

	template<typename ComponentType>
	ComponentType& ECS::getComponent(entity_id_t id)
	{
		assert(component_manager_.hasComponent<ComponentType>(id));
		return component_manager_.getComponent<ComponentType>(id);
	}

	template<typename ComponentType>
	const ComponentType& ECS::getComponent(entity_id_t id) const
	{
		assert(component_manager_.hasComponent<ComponentType>(id));
		return component_manager_.getComponent<ComponentType>(id);
	}

	template<typename T, typename... Args>
	requires std::constructible_from<T, Args...>
	auto ECS::registerSystem(Args&& ... args)
	{
		return system_manager_.registerSystem<T>(std::forward<Args>(args)...);
	}

	template<typename ComponentType>
	void magmatic::ecs::ECS::updateEntitySignature(entity_id_t id)
	{
		const auto component_type_id = component_manager_.getComponentTypeId<ComponentType>();
		auto mask = entity_manager_.getComponentMask(id);

		if(!mask.test(component_type_id))
		{
			mask.set(component_type_id, true);
			entity_manager_.setComponentMask(id, mask);
			system_manager_.updateEntityMask(id, mask);

			spdlog::debug("Magmatic: Created new component for entity");
		}
	}
}

#endif //MAGMATIC_ECS_HPP
