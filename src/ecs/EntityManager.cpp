#include "ecs/EntityManager.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

magmatic::ecs::EntityManager::EntityManager()
{
	entities.resize(MAX_ENTITIES_COUNT);
}

magmatic::ecs::entity_id_t magmatic::ecs::EntityManager::addEntity()
{
	entity_id_t new_id;

	if(!free_IDs.empty())
	{
		new_id = free_IDs.front();
		free_IDs.pop();
	}
	else
	{
		new_id = last_free_id;
		++last_free_id;
	}

	assert(new_id < entities.size());

	entities[new_id].mask.reset();
	entities[new_id].exist = true;
	++size_;
	return new_id;
}

void magmatic::ecs::EntityManager::removeEntity(entity_id_t id)
{
	checkEntityId(id);

	entities[id].exist = false;
	free_IDs.push(id);
	--size_;
}

bool magmatic::ecs::EntityManager::entityExists(entity_id_t id) const noexcept
{
	if(id >= entities.size())
	{
		return false;
	}
	return entities[id].exist;
}

void magmatic::ecs::EntityManager::setComponentMask(entity_id_t id, components_mask_t mask)
{
	checkEntityId(id);

	entities[id].mask = mask;
}

const magmatic::ecs::components_mask_t&
magmatic::ecs::EntityManager::getComponentMask(entity_id_t id) const
{
	checkEntityId(id);
	return entities.at(id).mask;
}

inline void magmatic::ecs::EntityManager::checkEntityId(entity_id_t id) const
{
	if(id >= entities.size() || !entities[id].exist)
	{
		spdlog::error("Magmatic: Access at invalid entityID");
		throw std::out_of_range("Invalid entity_id_t");
	}
}

std::size_t magmatic::ecs::EntityManager::size() const noexcept
{
	return size_;
}

std::size_t magmatic::ecs::EntityManager::capacity() const noexcept
{
	return entities.capacity();
}

magmatic::ecs::EntityManager::const_iterator magmatic::ecs::EntityManager::begin() const noexcept
{
	return Iterator(this, 0);
}

magmatic::ecs::EntityManager::const_iterator magmatic::ecs::EntityManager::cbegin() const noexcept
{
	return Iterator(this, 0);
}

magmatic::ecs::EntityManager::const_iterator magmatic::ecs::EntityManager::end() const noexcept
{
	return Iterator(this, entities.capacity());
}

magmatic::ecs::EntityManager::const_iterator magmatic::ecs::EntityManager::cend() const noexcept
{
	return Iterator(this, entities.capacity());
}


magmatic::ecs::EntityManager::MaskedView::iterator magmatic::ecs::EntityManager::MaskedView::begin() const noexcept
{
	return MaskedIterator(&manager, mask, 0);
}

magmatic::ecs::EntityManager::MaskedView::iterator magmatic::ecs::EntityManager::MaskedView::cbegin() const noexcept
{
	return MaskedIterator(&manager, mask, 0);
}

magmatic::ecs::EntityManager::MaskedView::iterator magmatic::ecs::EntityManager::MaskedView::end() const noexcept
{
	return MaskedIterator(&manager, mask, manager.capacity());
}

magmatic::ecs::EntityManager::MaskedView::iterator magmatic::ecs::EntityManager::MaskedView::cend() const noexcept
{
	return MaskedIterator(&manager, mask, manager.capacity());
}