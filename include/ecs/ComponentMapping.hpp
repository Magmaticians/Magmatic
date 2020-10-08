#ifndef MAGMATIC_COMPONENTMAPPING_HPP
#define MAGMATIC_COMPONENTMAPPING_HPP

#include "ECSTypes.hpp"
#include <unordered_map>
#include <map>
#include <spdlog/spdlog.h>


namespace magmatic::ecs
{
	class BaseComponentMapping
	{
	public:
		virtual ~BaseComponentMapping() = default;

		virtual void remove(entity_id_t id) = 0;
	};

	template<typename T>
	class ComponentMapping : public BaseComponentMapping
	{
	public:
		using component_id_t = std::size_t;

		ComponentMapping()
		{
			components_.reserve(MAX_ENTITIES_COUNT);
		};

		void insert(entity_id_t id, T&& component);
		void remove(entity_id_t id) override;

		T& get(entity_id_t id);
		const T& get(entity_id_t id) const;

		[[nodiscard]] bool exist(entity_id_t) const noexcept;

	private:
		std::vector<T> components_;
		std::unordered_map<entity_id_t, component_id_t> entity_to_component_;
		std::unordered_map<component_id_t, entity_id_t> component_to_entity_;
	};

	template<typename T>
	void ComponentMapping<T>::insert(entity_id_t id, T&& component)
	{
		const auto size = components_.size();
		assert(size < MAX_ENTITIES_COUNT && "Component limit exceeded");
		if(entity_to_component_.contains(id))
		{
			spdlog::error("Magmatic: Failed to add Component. Already exists");
			throw std::runtime_error("Component already exist");
		}

		components_.emplace_back(std::forward<T>(component));
		entity_to_component_[id] = size;
		component_to_entity_[size] = id;
	}

	template<typename T>
	void ComponentMapping<T>::remove(entity_id_t id)
	{
		if(!exist(id))
		{
			spdlog::warn("Magmatic: Failed to remove component od entity. Not exist");
			return;
		}

		const auto size = components_.size();

		// todo: check if drop on last index
		const component_id_t old_index = entity_to_component_[id];
		std::swap(components_[old_index], components_[size - 1]);

		const entity_id_t end_component_id = component_to_entity_[size - 1];
		entity_to_component_[end_component_id] = old_index;
		component_to_entity_[old_index] = end_component_id;

		entity_to_component_.erase(id);
		component_to_entity_.erase(size - 1);

		components_.pop_back();
	}

	template<typename T>
	T &ComponentMapping<T>::get(entity_id_t id)
	{
		try
		{
			const component_id_t component_id =  entity_to_component_.at(id);
			return components_[component_id];
		}
		catch (const std::out_of_range&)
		{
			spdlog::error("Magmatic: Failed to get component of entity. Not exist");
			throw;
		}
	}

	template<typename T>
	const T& ComponentMapping<T>::get(entity_id_t id) const
	{
		try
		{
			const component_id_t component_id =  entity_to_component_.at(id);
			return components_[component_id];
		}
		catch (const std::out_of_range&)
		{
			spdlog::error("Magmatic: Failed to get component of entity. Not exist");
			throw;
		}
	}

	template<typename T>
	bool ComponentMapping<T>::exist(entity_id_t id) const noexcept
	{
		return entity_to_component_.contains(id);
	}
}

#endif //MAGMATIC_COMPONENTMAPPING_HPP
