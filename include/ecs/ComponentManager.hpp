#ifndef MAGMATIC_COMPONENTMANAGER_HPP
#define MAGMATIC_COMPONENTMANAGER_HPP

#include "ecs/ComponentMapping.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>

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
		bool componentRegistered() const noexcept;

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
		struct MappingEntry
		{
			ComponentTypeID id;
			std::shared_ptr<BaseComponentMapping> mapping;
		};

		std::unordered_map<std::type_index, MappingEntry> mappings;

		template<typename T>
		ComponentMappingPtr<T> getMapping() const;

		template<typename T>
		static std::type_index mappingID();
	};

	template<typename T>
	bool ComponentManager::registerComponent()
	{
		const std::type_index mapping_id = mappingID<T>();
		assert(!mappings.contains(mapping_id));

		mappings.insert(
				{
					mapping_id,
					{
						lastID,
						std::make_shared<ComponentMapping<T>>()
					}
				});

		++lastID;
		return true;
	}

	template<typename T>
	ComponentManager::ComponentTypeID ComponentManager::getComponentTypeID() const
	{
		const std::type_index mapping_id = typeid(T);
		return mappings.at(mapping_id).id;
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
		const std::type_index mapping_id = typeid(T);
		try
		{
			const auto& mapping = mappings.at(mapping_id).mapping;
			return std::static_pointer_cast<ComponentMapping<T>>(mapping);
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

	template<typename T>
	bool ComponentManager::componentRegistered() const noexcept
	{
		const std::type_index mapping_id = typeid(T);
		return mappings.contains(mapping_id);
	}

	template<typename T>
	std::type_index ComponentManager::mappingID()
	{
		//todo: compile time?
		const std::type_info& type = typeid(T);
		return std::type_index(type);
	}
}

#endif //MAGMATIC_COMPONENTMANAGER_HPP
