#include <gtest/gtest.h>
#include "ecs/EntityManager.hpp"

class EntityManagerTest : public ::testing::Test
{
protected:
	magmatic::ecs::EntityManager manager;
};

TEST_F(EntityManagerTest, addEntity)
{
	ASSERT_EQ(0, manager.addEntity());
	ASSERT_EQ(1, manager.addEntity());
	ASSERT_EQ(2, manager.addEntity());
	ASSERT_EQ(3, manager.addEntity());
}

TEST_F(EntityManagerTest, removeEntity)
{
	const auto id_1 = manager.addEntity();
	const auto id_2 = manager.addEntity();

	manager.removeEntity(id_1);
	ASSERT_FALSE(manager.entityExists(id_1));
	ASSERT_TRUE(manager.entityExists(id_2));
	manager.removeEntity(id_2);
	ASSERT_FALSE(manager.entityExists(id_2));

	ASSERT_THROW(manager.removeEntity(id_2), std::out_of_range);
}

TEST_F(EntityManagerTest, reuseEntityID)
{
	manager.addEntity();
	const auto id_2 = manager.addEntity();
	manager.addEntity();

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

	magmatic::ecs::EntityManager::ComponentsMask mask;
	mask.set(3);

	manager.setComponentMask(id_2, mask);

	ASSERT_TRUE(manager.getComponentMask(id_1).none());
	ASSERT_EQ(mask, manager.getComponentMask(id_2));
	ASSERT_THROW(auto _ = manager.getComponentMask(8), std::out_of_range);
}