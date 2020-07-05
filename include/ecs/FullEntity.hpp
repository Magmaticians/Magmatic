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

		FullEntity(EntityID id, ComponentManager& manager);

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

	private:
		EntityID id_;

		ComponentManager& manager_;
	};

	FullEntity::FullEntity(FullEntity::EntityID id, ComponentManager& manager)
	:id_(id), manager_(manager) {}

	template<typename ComponentType>
	ComponentType& FullEntity::get()
	{
		return manager_.getComponent<ComponentType>(id_);
	}

	template<typename ComponentType>
	const ComponentType &FullEntity::get() const
	{
		return manager_.getComponent<ComponentType>(id_);
	}

	template<typename ComponentType, typename... Args>
	requires std::constructible_from<ComponentManager, Args...>
	void FullEntity::set(Args &&... args)
	{
		manager_.addComponent<ComponentType>(std::forward<Args>(args)...);
	}

	template<typename ComponentType>
	void FullEntity::set(ComponentType&& component)
	{
		manager_.addComponent<ComponentType>(std::forward(component));
	}

	template<typename ComponentType>
	bool FullEntity::has() const
	{
		return manager_.hasComponent<ComponentType>(id_);
	}
}

#endif //MAGMATIC_FULLENTITY_HPP
