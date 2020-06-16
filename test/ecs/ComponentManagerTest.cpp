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

	ASSERT_EQ(0, manager.getComponentTypeID<int>());
	ASSERT_EQ(1, manager.getComponentTypeID<double>());
}


TEST_F(ComponentManagerTest, throwOnQueryIDNotInsertedComponent)
{
	ASSERT_THROW(manager.getComponentTypeID<int>(), std::out_of_range);
	ASSERT_THROW(manager.getComponentTypeID<float>(), std::out_of_range);
}


TEST_F(ComponentManagerTest, throwOnAddNotRegisteredComponent)
{
	ASSERT_THROW(manager.addComponent(0, 12), std::out_of_range);
	ASSERT_THROW(manager.addComponent(3, 5.0), std::out_of_range);
}



TEST_F(ComponentManagerTest, storingComponents)
{
	manager.registerComponent<int>();
	manager.addComponent(0, 5);
	manager.addComponent(3, 3);

	manager.registerComponent<double>();
	manager.addComponent(0, 6.7);
	manager.addComponent(2, 5.6);

	ASSERT_EQ(manager.getComponent<double>(0), 6.7);
	ASSERT_EQ(manager.getComponent<double>(2), 5.6);

	ASSERT_EQ(manager.getComponent<int>(0), 5);
	ASSERT_EQ(manager.getComponent<int>(3), 3);
}

TEST_F(ComponentManagerTest, throwOnAddAlreadyAddedComponent)
{
	manager.registerComponent<int>();
	manager.addComponent(0, 5);
	ASSERT_THROW(manager.addComponent(0, 3), std::runtime_error);
}

TEST_F(ComponentManagerTest, removingAllComponentOfEntity)
{
	manager.registerComponent<int>();
	manager.addComponent(0, 5);
	manager.addComponent(3, 3);

	manager.registerComponent<double>();
	manager.addComponent(0, 6.7);

	manager.removeEntityComponents(0);

	ASSERT_THROW(manager.getComponent<int>(0), std::out_of_range);
	ASSERT_THROW(manager.getComponent<double>(0), std::out_of_range);
	ASSERT_EQ(3, manager.getComponent<int>(3));
}


TEST_F(ComponentManagerTest, removingSingleComponent)
{
	manager.registerComponent<int>();
	manager.addComponent(0, 5);
	manager.addComponent(3, 3);

	manager.removeComponent<int>(0);

	ASSERT_THROW(manager.getComponent<int>(0), std::out_of_range);
	ASSERT_EQ(3, manager.getComponent<int>(3));
}


TEST_F(ComponentManagerTest, constGetComponent)
{
	manager.registerComponent<int>();
	manager.addComponent(0, 5);

	const auto& const_manager = manager;
	ASSERT_EQ(5, const_manager.getComponent<int>(0));
	ASSERT_THROW(const_manager.getComponent<int>(1), std::out_of_range);

}
