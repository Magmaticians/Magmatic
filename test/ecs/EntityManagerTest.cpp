#include <gtest/gtest.h>
#include "ecs/EntityManager.hpp"
#include "ecs/ECSTypes.hpp"

class EntityManagerTest : public ::testing::Test
{
protected:
	magmatic::ecs::EntityManager manager;
};

TEST_F(EntityManagerTest, init)
{
	ASSERT_EQ(magmatic::ecs::MAX_ENTITIES_COUNT, manager.capacity());
	ASSERT_EQ(0, manager.size());
}

TEST_F(EntityManagerTest, addEntity)
{
	ASSERT_EQ(0, manager.addEntity());
	ASSERT_EQ(1, manager.size());

	ASSERT_EQ(1, manager.addEntity());
	ASSERT_EQ(2, manager.size());

	ASSERT_EQ(2, manager.addEntity());
	ASSERT_EQ(3, manager.size());

	ASSERT_EQ(3, manager.addEntity());
}

TEST_F(EntityManagerTest, removeEntity)
{
	const auto id_1 = manager.addEntity();
	const auto id_2 = manager.addEntity();

	manager.removeEntity(id_1);
	ASSERT_FALSE(manager.entityExists(id_1));
	ASSERT_TRUE(manager.entityExists(id_2));
	ASSERT_EQ(1, manager.size());
	manager.removeEntity(id_2);
	ASSERT_FALSE(manager.entityExists(id_2));
	ASSERT_EQ(0, manager.size());

	ASSERT_THROW(manager.removeEntity(id_2), std::out_of_range);
}

TEST_F(EntityManagerTest, reuseEntityID)
{
	static_cast<void>(manager.addEntity());
	const auto id_2 = manager.addEntity();
	static_cast<void>(manager.addEntity());

	manager.removeEntity(id_2);

	ASSERT_EQ(id_2, manager.addEntity());
}

TEST_F(EntityManagerTest, entityExists)
{
	const auto id = manager.addEntity();

	ASSERT_TRUE(manager.entityExists(id));
	ASSERT_FALSE(manager.entityExists(id+1));
}

TEST_F(EntityManagerTest, componentMaskDefaultEmpty)
{
	const auto id_1 = manager.addEntity();
	ASSERT_TRUE(manager.getComponentMask(id_1).none());
}

TEST_F(EntityManagerTest, componentMaskStoringAndLoading)
{
	const auto id_1 = manager.addEntity();
	const auto id_2 = manager.addEntity();

	magmatic::ecs::components_mask_t mask;
	mask.set(3);

	manager.setComponentMask(id_2, mask);

	ASSERT_TRUE(manager.getComponentMask(id_1).none());
	ASSERT_EQ(mask, manager.getComponentMask(id_2));
	ASSERT_THROW(static_cast<void>(manager.getComponentMask(8)), std::out_of_range);
}

TEST_F(EntityManagerTest, iteratorAdvance)
{
	const auto id_1 = manager.addEntity();
	const auto id_2 = manager.addEntity();

	auto iter_1 = manager.begin();
	auto iter_2 = manager.cbegin();

	ASSERT_EQ(id_1, *iter_1);
	ASSERT_EQ(id_1, *iter_2);

	ASSERT_EQ(id_2, *++iter_1);
	ASSERT_EQ(id_2, *++iter_2);

	ASSERT_EQ(id_2, *iter_1++);
	ASSERT_EQ(id_2, *iter_2++);

	ASSERT_EQ(2, std::distance(manager.begin(), manager.end()));
	ASSERT_EQ(2, std::distance(manager.cbegin(), manager.cend()));

	ASSERT_EQ(2, std::distance(std::begin(manager), std::end(manager)));
	ASSERT_EQ(2, std::distance(std::cbegin(manager), std::cend(manager)));
}

TEST_F(EntityManagerTest, iteratorAdvanceWithRemoved)
{
	const auto id_1 = manager.addEntity();
	const auto id_2 = manager.addEntity();
	const auto id_3 = manager.addEntity();

	manager.removeEntity(id_2);

	auto iter_1 = manager.begin();
	auto iter_2 = manager.cbegin();

	ASSERT_EQ(id_1, *iter_1);
	ASSERT_EQ(id_1, *iter_2);

	ASSERT_EQ(id_3, *++iter_1);
	ASSERT_EQ(id_3, *++iter_2);

	ASSERT_EQ(id_3, *iter_1++);
	ASSERT_EQ(id_3, *iter_2++);

	ASSERT_EQ(2, std::distance(manager.begin(), manager.end()));
	ASSERT_EQ(2, std::distance(manager.cbegin(), manager.cend()));

	ASSERT_EQ(2, std::distance(std::begin(manager), std::end(manager)));
	ASSERT_EQ(2, std::distance(std::cbegin(manager), std::cend(manager)));
}

TEST_F(EntityManagerTest, maskedIteratorAdvance)
{
	const magmatic::ecs::components_mask_t mask(1);
	const magmatic::ecs::EntityManager::MaskedView view(manager, mask);

	const auto id_1 = manager.addEntity();
	static_cast<void>( manager.addEntity());
	const auto id_3 = manager.addEntity();

	manager.setComponentMask(id_1, mask);
	manager.setComponentMask(id_3, mask);

	auto iter_1 = view.begin();
	auto iter_2 = view.cbegin();

	ASSERT_EQ(id_1, *iter_1);
	ASSERT_EQ(id_1, *iter_2);

	ASSERT_EQ(id_3, *++iter_1);
	ASSERT_EQ(id_3, *++iter_2);

	ASSERT_EQ(id_3, *iter_1++);
	ASSERT_EQ(id_3, *iter_2++);

	ASSERT_EQ(2, std::distance(view.begin(), view.end()));
	ASSERT_EQ(2, std::distance(view.cbegin(), view.cend()));

	ASSERT_EQ(2, std::distance(std::begin(view), std::end(view)));
	ASSERT_EQ(2, std::distance(std::cbegin(view), std::cend(view)));
}

TEST_F(EntityManagerTest, maskedIteratorAdvanceWithRemoved)
{
	const magmatic::ecs::components_mask_t mask(1);
	const magmatic::ecs::EntityManager::MaskedView view(manager, mask);

	const auto id_1 = manager.addEntity();
	static_cast<void>(manager.addEntity());
	const auto id_3 = manager.addEntity();
	const auto id_4 = manager.addEntity();

	manager.setComponentMask(id_1, mask);
	manager.setComponentMask(id_3, mask);
	manager.setComponentMask(id_4, mask);

	manager.removeEntity(id_1);

	auto iter_1 = view.begin();
	auto iter_2 = view.cbegin();

	ASSERT_EQ(id_3, *iter_1);
	ASSERT_EQ(id_3, *iter_2);

	ASSERT_EQ(id_4, *++iter_1);
	ASSERT_EQ(id_4, *++iter_2);

	ASSERT_EQ(id_4, *iter_1++);
	ASSERT_EQ(id_4, *iter_2++);

	ASSERT_EQ(2, std::distance(view.begin(), view.end()));
	ASSERT_EQ(2, std::distance(view.cbegin(), view.cend()));

	ASSERT_EQ(2, std::distance(std::begin(view), std::end(view)));
	ASSERT_EQ(2, std::distance(std::cbegin(view), std::cend(view)));
}