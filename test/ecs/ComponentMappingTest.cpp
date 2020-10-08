#include <gtest/gtest.h>
#include <ecs/ComponentMapping.hpp>

class ComponentMappingTest : public ::testing::Test
{
protected:
	magmatic::ecs::ComponentMapping<int> mapping;
};

TEST_F(ComponentMappingTest, storingComponents)
{
	mapping.insert(0, 1);
	mapping.insert(2, 4);

	ASSERT_EQ(1, mapping.get(0));
	ASSERT_EQ(4, mapping.get(2));
}


TEST_F(ComponentMappingTest, throwOnStoringMoreThanOnce)
{
	mapping.insert(1, 1);
	ASSERT_THROW(mapping.insert(1, 1), std::runtime_error);
}

TEST_F(ComponentMappingTest, throwOnGetOnEmpty)
{
	ASSERT_THROW(mapping.get(0), std::out_of_range);
	mapping.insert(1, 1);
	mapping.remove(1);
	ASSERT_THROW(mapping.get(0), std::out_of_range);
}

TEST_F(ComponentMappingTest, constGet)
{
	mapping.insert(0, 2);
	const auto& const_mapping = mapping;
	ASSERT_EQ(2, const_mapping.get(0));
}

TEST_F(ComponentMappingTest, removingComponents)
{
	mapping.insert(0, 1);
	mapping.insert(1, 2);

	mapping.remove(1);

	ASSERT_EQ(1, mapping.get(0));
	ASSERT_FALSE(mapping.exist(1));
}

TEST_F(ComponentMappingTest, componentExistance)
{
	mapping.insert(1, 2);

	ASSERT_FALSE(mapping.exist(0));
	ASSERT_TRUE(mapping.exist(1));
}