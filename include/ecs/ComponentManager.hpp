#ifndef MAGMATIC_COMPONENTMANAGER_HPP
#define MAGMATIC_COMPONENTMANAGER_HPP

#include "ComponentMapping.hpp"
#include "ECSTypes.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>

namespace magmatic::ecs
{
	class ComponentManager
	{
	public:
		using component_type_id_t = std::size_t;

		template<typename T>
		using component_mapping_ptr = std::shared_ptr<ComponentMapping<T>>;

		template<typename T>
		bool registerComponent();

		template<typename T>
		bool componentRegistered() const noexcept;

		template<typename T>
		component_type_id_t getComponentTypeId() const;

		template<typename T, typename... Args>
		requires std::constructible_from<T, Args...>
		void addComponent(entity_id_t id, Args&& ... args);

		template<typename T>
		void removeComponent(entity_id_t id);

		template<typename T>
		T& getComponent(entity_id_t id);

		template<typename T>
		const T& getComponent(entity_id_t id) const;

		template<typename T>
		bool hasComponent(entity_id_t id) const;

		void removeEntityComponents(entity_id_t id) noexcept;

	private:
		component_type_id_t last_id_ = 0;
		struct MappingEntry
		{
			component_type_id_t id;
			std::shared_ptr<BaseComponentMapping> mapping;
		};

		std::unordered_map<std::type_index, MappingEntry> mappings_;

		template<typename T>
		component_mapping_ptr<T> getMapping() const;

		template<typename T>
		static std::type_index mappingId();
	};

	template<typename T>
	bool ComponentManager::registerComponent()
	{
		const std::type_index mapping_id = mappingId<T>();
		assert(!mappings_.contains(mapping_id));

		mappings_.insert(
				{
					mapping_id,
					{
							last_id_,
							std::make_shared<ComponentMapping<T>>()
					}
				});

		++last_id_;
		return true;
	}

	template<typename T>
	ComponentManager::component_type_id_t ComponentManager::getComponentTypeId() const
	{
		const std::type_index mapping_id = typeid(T);
		return mappings_.at(mapping_id).id;
	}

	template<typename T, typename... Args>
	requires std::constructible_from<T, Args...>
	void ComponentManager::addComponent(entity_id_t id, Args &&... args)
	{
		getMapping<T>()->insert(id, T(std::forward<Args>(args)...));
	}

	template<typename T>
	void ComponentManager::removeComponent(entity_id_t id)
	{
		getMapping<T>()->remove(id);
	}

	template<typename T>
	ComponentManager::component_mapping_ptr<T> ComponentManager::getMapping() const
	{
		const std::type_index mapping_id = typeid(T);
		try
		{
			const auto& mapping = mappings_.at(mapping_id).mapping;
			return std::static_pointer_cast<ComponentMapping<T>>(mapping);
		}
		catch (const std::out_of_range&)
		{
			spdlog::error("Magmatic: ECS Component not registered");
			throw;
		}
	}

	template<typename T>
	T &ComponentManager::getComponent(entity_id_t id)
	{
		return getMapping<T>()->get(id);
	}

	template<typename T>
	bool ComponentManager::hasComponent(entity_id_t id) const
	{
		return getMapping<T>()->exist(id);
	}

	template<typename T>
	const T &ComponentManager::getComponent(entity_id_t id) const
	{
		return getMapping<T>()->get(id);
	}

	template<typename T>
	bool ComponentManager::componentRegistered() const noexcept
	{
		const std::type_index mapping_id = typeid(T);
		return mappings_.contains(mapping_id);
	}

	template<typename T>
	std::type_index ComponentManager::mappingId()
	{
		//todo: compile time?
		const std::type_info& type = typeid(T);
		return std::type_index(type);
	}
}

#endif //MAGMATIC_COMPONENTMANAGER_HPP
