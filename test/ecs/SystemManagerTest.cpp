#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <gmock/gmock.h>
#include "ecs/ComponentManager.hpp"
#include "ecs/SystemManager.hpp"

using magmatic::ecs::System;
using ComponentMask = magmatic::ecs::EntityManager::ComponentsMask;

class EmptySystem : public magmatic::ecs::System
{
public:
	MOCK_METHOD2(update, void(const std::chrono::duration<int64_t, std::micro>& delta, const magmatic::ecs::ComponentManager& component_manager));
};

class SystemManagerTest : public ::testing::Test
{
protected:
	magmatic::ecs::SystemManager manager;

	class EmptySystem2: public EmptySystem {};

	const char* system_name = typeid(EmptySystem).name();
	const char* system2_name = typeid(EmptySystem2).name();
};

TEST_F(SystemManagerTest, registeringSystem)
{
	manager.registerSystem<EmptySystem>();
	const auto registered = manager.getAllSystemsID();

	EXPECT_THAT(registered, ::testing::ElementsAre(system_name));

	manager.registerSystem<EmptySystem2>();
	const auto new_registered = manager.getAllSystemsID();

	EXPECT_THAT(new_registered, ::testing::UnorderedElementsAre(system_name, system2_name));
}


TEST_F(SystemManagerTest, assertOnDoubleRegisteringSystem)
{
	manager.registerSystem<EmptySystem>();
	ASSERT_DEATH(manager.registerSystem<EmptySystem>(), "");
}


TEST_F(SystemManagerTest, getSystem)
{
	manager.registerSystem<EmptySystem>();
	ASSERT_NE(nullptr, manager.getSystem<EmptySystem>().get());
}


TEST_F(SystemManagerTest, setMask)
{
	manager.registerSystem<EmptySystem>();
	const ComponentMask mask1{1};
	const ComponentMask mask2{2};

	ASSERT_TRUE(manager.setSystemMask<EmptySystem>(mask1).none());
	ASSERT_EQ(mask1, manager.setSystemMask<EmptySystem>(mask2));
}

TEST_F(SystemManagerTest, addEntity)
{
	class EmptySystem3: public EmptySystem {};
	class EmptySystem4: public EmptySystem {};

	manager.registerSystem<EmptySystem>();
	manager.registerSystem<EmptySystem2>();
	manager.registerSystem<EmptySystem3>();
	manager.registerSystem<EmptySystem4>();

	ComponentMask mask;
	mask.set();

	manager.setSystemMask<EmptySystem>(mask);
	manager.setSystemMask<EmptySystem2>(2);
	manager.setSystemMask<EmptySystem3>(4);


	manager.updateEntityMask(0, 2);
	manager.updateEntityMask(3, 4);
	manager.updateEntityMask(6, 5);

	const auto system = manager.getSystem<EmptySystem>();
	const auto system2 = manager.getSystem<EmptySystem2>();
	const auto system3 = manager.getSystem<EmptySystem3>();
	const auto system4 = manager.getSystem<EmptySystem4>();

	ASSERT_TRUE(system->handled_entities.empty());
	EXPECT_THAT(system2->handled_entities, ::testing::UnorderedElementsAre(0));
	EXPECT_THAT(system3->handled_entities, ::testing::UnorderedElementsAre(3, 6));
	EXPECT_THAT(system4->handled_entities, ::testing::UnorderedElementsAre(0, 3, 6));
}

TEST_F(SystemManagerTest, updateEntity)
{
	manager.registerSystem<EmptySystem>();
	manager.registerSystem<EmptySystem2>();

	manager.setSystemMask<EmptySystem2>(4);

	manager.updateEntityMask(0, 4);
	manager.updateEntityMask(0, 1);


	const auto system = manager.getSystem<EmptySystem>();
	const auto system2 = manager.getSystem<EmptySystem2>();

	ASSERT_FALSE(system->handled_entities.empty());
	EXPECT_TRUE(system2->handled_entities.empty());
}


TEST_F(SystemManagerTest, removeEntity)
{
	manager.registerSystem<EmptySystem>();
	manager.registerSystem<EmptySystem2>();

	manager.setSystemMask<EmptySystem2>(2);

	manager.updateEntityMask(0, 2);
	manager.updateEntityMask(3, 4);

	manager.entityRemoved(0);

	const auto system = manager.getSystem<EmptySystem>();
	const auto system2 = manager.getSystem<EmptySystem2>();

	EXPECT_THAT(system->handled_entities, ::testing::UnorderedElementsAre(3));
	ASSERT_TRUE(system2->handled_entities.empty());
}

TEST_F(SystemManagerTest, updateSystem)
{
	using magmatic::ecs::ComponentManager;
	ComponentManager component_manager;
	manager.registerSystem<EmptySystem>();
	manager.registerSystem<EmptySystem2>();

	manager.entityRemoved(0);

	const auto system = manager.getSystem<EmptySystem>();
	const auto system2 = manager.getSystem<EmptySystem2>();

	std::chrono::duration<int64_t, std::micro> zero_delta{0};
	EXPECT_CALL(*system, update(zero_delta, ::testing::Ref(component_manager)))
		.Times(1);
	EXPECT_CALL(*system2, update(zero_delta, ::testing::Ref(component_manager)))
		.Times(1);

	manager.update(zero_delta, component_manager);

	std::chrono::duration<int64_t, std::micro> two_delta{2};
	EXPECT_CALL(*system, update(two_delta, ::testing::Ref(component_manager)))
			.Times(1);
	EXPECT_CALL(*system2, update(two_delta, ::testing::Ref(component_manager)))
			.Times(1);

	manager.update(two_delta, component_manager);
}
