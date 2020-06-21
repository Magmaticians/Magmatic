#ifndef MAGMATIC_COMPONENTMANAGER_HPP
#define MAGMATIC_COMPONENTMANAGER_HPP

#include "ecs/ComponentMapping.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace magmatic::ecs
{
	class ComponentManager
	{
	public:
		using ComponentTypeID = std::size_t;
		using ComponentTypeName = std::string;
		template<typename T>
		using ComponentMappingPtr = std::shared_ptr<ComponentMapping<T>>;
		using EntityID = EntityManager::EntityID;

		template<typename T>
		bool registerComponent();

		template<typename T>
		ComponentTypeID getComponentTypeID() const;

		template<typename T>
		void addComponent(EntityID id, T&& component);

		template<typename T, typename... Args>
		void addComponent(EntityID id, Args&& ... args);

		template<typename T>
		void removeComponent(EntityID id);

		template<typename T>
		T& getComponent(EntityID id);

		template<typename T>
		const T& getComponent(EntityID id) const;

		template<typename T>
		bool hasComponent(EntityID id) const;

		void removeEntityComponents(EntityID id) noexcept;

	private:
		ComponentTypeID lastID = 0;
		std::unordered_map<ComponentTypeName, ComponentTypeID> registered_components;
		std::unordered_map<ComponentTypeName, std::shared_ptr<BaseComponentMapping>> mappings;

		template<typename T>
		ComponentMappingPtr<T> getMapping() const;
	};

	template<typename T>
	bool ComponentManager::registerComponent()
	{
		const auto name = typeid(T).name();
		assert(!registered_components.contains(name));

		registered_components.insert({name, lastID});
		mappings.insert(std::make_pair(name, std::make_shared<ComponentMapping<T>>()));

		++lastID;
		return true;
	}

	template<typename T>
	ComponentManager::ComponentTypeID ComponentManager::getComponentTypeID() const
	{
		const auto name = typeid(T).name();

		return registered_components.at(name);
	}

	template<typename T>
	void ComponentManager::addComponent(ComponentManager::EntityID id, T&& component)
	{
		getMapping<T>()->insert(id, std::forward<T>(component));
	}

	template<typename T, typename... Args>
	void ComponentManager::addComponent(ComponentManager::EntityID id, Args &&... args)
	{
		getMapping<T>()->insert(id, T(std::forward<Args>(args)...));
	}

	template<typename T>
	void ComponentManager::removeComponent(ComponentManager::EntityID id)
	{
		getMapping<T>()->remove(id);
	}

	template<typename T>
	ComponentManager::ComponentMappingPtr<T> ComponentManager::getMapping() const
	{
		const auto name = typeid(T).name();
		try
		{
			return std::static_pointer_cast<ComponentMapping<T>>(mappings.at(name));
		}
		catch (const std::out_of_range&)
		{
			spdlog::error("Magmatic: ECS Component not registered");
			throw;
		}
	}

	template<typename T>
	T &ComponentManager::getComponent(ComponentManager::EntityID id)
	{
		return getMapping<T>()->get(id);
	}

	template<typename T>
	bool ComponentManager::hasComponent(EntityID id) const
	{
		return getMapping<T>()->exist(id);
	}

	template<typename T>
	const T &ComponentManager::getComponent(ComponentManager::EntityID id) const
	{
		return getMapping<T>()->get(id);
	}
}

#endif //MAGMATIC_COMPONENTMANAGER_HPP
