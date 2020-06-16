#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <gmock/gmock.h>
#include <ecs/SystemManager.hpp>

using magmatic::ecs::System;
using ComponentMask = magmatic::ecs::EntityManager::ComponentsMask;

class SystemManagerTest : public ::testing::Test
{
protected:
	magmatic::ecs::SystemManager manager;

	class System2: public System {};

	const char* system_name = typeid(System).name();
	const char* system2_name = typeid(System2).name();
};

TEST_F(SystemManagerTest, registeringSystem)
{
	manager.registerSystem<System>();
	const auto registered = manager.getAllSystemsID();

	EXPECT_THAT(registered, ::testing::ElementsAre(system_name));

	manager.registerSystem<System2>();
	const auto new_registered = manager.getAllSystemsID();

	EXPECT_THAT(new_registered, ::testing::UnorderedElementsAre(system_name, system2_name));
}


TEST_F(SystemManagerTest, assertOnDoubleRegisteringSystem)
{
	manager.registerSystem<System>();
	ASSERT_DEATH(manager.registerSystem<System>(), "");
}


TEST_F(SystemManagerTest, getSystem)
{
	manager.registerSystem<System>();
	ASSERT_NE(nullptr, manager.getSystem<System>().get());
}


TEST_F(SystemManagerTest, setMask)
{
	manager.registerSystem<System>();
	const ComponentMask mask1{1};
	const ComponentMask mask2{2};

	ASSERT_TRUE(manager.setSystemMask<System>(mask1).none());
	ASSERT_EQ(mask1, manager.setSystemMask<System>(mask2));
}

TEST_F(SystemManagerTest, addEntity)
{
	class System3: public System {};
	class System4: public System {};

	manager.registerSystem<System>();
	manager.registerSystem<System2>();
	manager.registerSystem<System3>();
	manager.registerSystem<System4>();

	ComponentMask mask;
	mask.set();

	manager.setSystemMask<System>(mask);
	manager.setSystemMask<System2>(2);
	manager.setSystemMask<System3>(4);


	manager.updateEntityMask(0, 2);
	manager.updateEntityMask(3, 4);
	manager.updateEntityMask(6, 5);

	const auto system = manager.getSystem<System>();
	const auto system2 = manager.getSystem<System2>();
	const auto system3 = manager.getSystem<System3>();
	const auto system4 = manager.getSystem<System4>();

	ASSERT_TRUE(system->handled_entities.empty());
	EXPECT_THAT(system2->handled_entities, ::testing::UnorderedElementsAre(0));
	EXPECT_THAT(system3->handled_entities, ::testing::UnorderedElementsAre(3, 6));
	EXPECT_THAT(system4->handled_entities, ::testing::UnorderedElementsAre(0, 3, 6));
}

TEST_F(SystemManagerTest, updateEntity)
{
	manager.registerSystem<System>();
	manager.registerSystem<System2>();

	manager.setSystemMask<System2>(4);

	manager.updateEntityMask(0, 4);
	manager.updateEntityMask(0, 1);


	const auto system = manager.getSystem<System>();
	const auto system2 = manager.getSystem<System2>();

	ASSERT_FALSE(system->handled_entities.empty());
	EXPECT_TRUE(system2->handled_entities.empty());
}


TEST_F(SystemManagerTest, removeEntity)
{
	manager.registerSystem<System>();
	manager.registerSystem<System2>();

	manager.setSystemMask<System2>(2);

	manager.updateEntityMask(0, 2);
	manager.updateEntityMask(3, 4);

	manager.entityRemoved(0);

	const auto system = manager.getSystem<System>();
	const auto system2 = manager.getSystem<System2>();

	EXPECT_THAT(system->handled_entities, ::testing::UnorderedElementsAre(3));
	ASSERT_TRUE(system2->handled_entities.empty());
}
