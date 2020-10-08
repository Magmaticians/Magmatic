#include <gtest/gtest.h>
#include <ecs/ComponentManager.hpp>


class ComponentManagerTest : public ::testing::Test
{
protected:
	magmatic::ecs::ComponentManager manager;
};

TEST_F(ComponentManagerTest, registerComponentType)
{

	manager.registerComponent<int>();

	manager.registerComponent<double>();

	ASSERT_TRUE(manager.componentRegistered<int>());
	ASSERT_TRUE(manager.componentRegistered<const int&>());
	ASSERT_FALSE(manager.componentRegistered<float>());
	ASSERT_EQ(0, manager.getComponentTypeId<int>());
	ASSERT_EQ(1, manager.getComponentTypeId<double>());
	ASSERT_DEATH(manager.registerComponent<int>(), "");
}


TEST_F(ComponentManagerTest, throwOnQueryIDNotInsertedComponent)
{
	ASSERT_THROW(manager.getComponentTypeId<int>(), std::out_of_range);
	ASSERT_THROW(manager.getComponentTypeId<float>(), std::out_of_range);
}


TEST_F(ComponentManagerTest, throwOnAddNotRegisteredComponent)
{
	ASSERT_THROW(manager.addComponent<int>(0, 12), std::out_of_range);
	ASSERT_THROW(manager.addComponent<float>(3, 5.0), std::out_of_range);
}

TEST_F(ComponentManagerTest, storingComponents)
{
	manager.registerComponent<int>();
	manager.addComponent<int>(0, 5);
	manager.addComponent<int>(3, 3);

	manager.registerComponent<double>();
	manager.addComponent<double>(0, 6.7);
	manager.addComponent<double>(2, 5.6);

	using Pair = std::pair<int, int>;
	manager.registerComponent<Pair>();
	manager.addComponent<Pair>(0, 6, 7);
	manager.addComponent<Pair>(2, 3, 7);

	ASSERT_EQ(manager.getComponent<double>(0), 6.7);
	ASSERT_EQ(manager.getComponent<double>(2), 5.6);

	ASSERT_TRUE(manager.hasComponent<double>(0));
	ASSERT_TRUE(manager.hasComponent<double>(2));
	ASSERT_FALSE(manager.hasComponent<double>(3));

	ASSERT_EQ(manager.getComponent<int>(0), 5);
	ASSERT_EQ(manager.getComponent<int>(3), 3);

	ASSERT_TRUE(manager.hasComponent<int>(0));
	ASSERT_TRUE(manager.hasComponent<int>(3));
	ASSERT_FALSE(manager.hasComponent<int>(2));

	ASSERT_EQ(manager.getComponent<Pair>(0), std::make_pair(6, 7));
	ASSERT_EQ(manager.getComponent<Pair>(2), std::make_pair(3, 7));

	ASSERT_TRUE(std::is_reference<decltype(manager.getComponent<int>(3))>::value);
}

TEST_F(ComponentManagerTest, throwOnAddAlreadyAddedComponent)
{
	manager.registerComponent<int>();
	manager.addComponent<int>(0, 5);
	ASSERT_THROW(manager.addComponent<int>(0, 3), std::runtime_error);
}

TEST_F(ComponentManagerTest, removingAllComponentOfEntity)
{
	manager.registerComponent<int>();
	manager.addComponent<int>(0, 5);
	manager.addComponent<int>(3, 3);

	manager.registerComponent<double>();
	manager.addComponent<double>(0, 6.7);

	manager.removeEntityComponents(0);

	ASSERT_THROW(manager.getComponent<int>(0), std::out_of_range);
	ASSERT_THROW(manager.getComponent<double>(0), std::out_of_range);
	ASSERT_EQ(3, manager.getComponent<int>(3));
}


TEST_F(ComponentManagerTest, removingSingleComponent)
{
	manager.registerComponent<int>();
	manager.addComponent<int>(0, 5);
	manager.addComponent<int>(3, 3);

	manager.removeComponent<int>(0);

	ASSERT_THROW(manager.getComponent<int>(0), std::out_of_range);
	ASSERT_EQ(3, manager.getComponent<int>(3));
	ASSERT_FALSE(manager.hasComponent<int>(0));
}


TEST_F(ComponentManagerTest, constGetComponent)
{
	manager.registerComponent<int>();
	manager.addComponent<int>(0, 5);

	const auto& const_manager = manager;
	ASSERT_EQ(5, const_manager.getComponent<int>(0));
	ASSERT_THROW(const_manager.getComponent<int>(1), std::out_of_range);
	ASSERT_TRUE(std::is_reference<decltype(const_manager.getComponent<int>(1))>::value);
}
