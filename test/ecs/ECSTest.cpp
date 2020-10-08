#include <gtest/gtest.h>
#include "ecs/ECS.hpp"

using namespace magmatic::ecs;

class ECSTest : public ::testing::Test
{
protected:
	ECS ecs;
};


TEST_F(ECSTest, storing_entities)
{
	ASSERT_FALSE(ecs.entityExists(9));
	ASSERT_FALSE(ecs.entityExists(0));

	auto entity_1 = ecs.createEntity();
	auto entity_2 = ecs.createEntity();

	ASSERT_NE(entity_1, entity_2);

	ASSERT_TRUE(ecs.entityExists(entity_1));
	ASSERT_TRUE(ecs.entityExists(entity_2));

	ecs.removeEntity(entity_1);

	ASSERT_FALSE(ecs.entityExists(entity_1));
}


TEST_F(ECSTest, storing_components)
{
	const auto& const_ecs = ecs;

	struct Speed
	{
		int y;

		bool operator==(const Speed& other) const
		{
			return y == other.y;
		}
	};
	struct Position
	{
		int x;
		int y;

		bool operator==(const Position& other) const
		{
			return x == other.x && y == other.y;
		}
	};

	struct Acceleration
	{
		Acceleration(int x, int y, int z)
		: x(x), y(y), z(z)
		{}

		int x;
		int y;
		int z;

		bool operator==(const Acceleration& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}
	};

	auto entity_1 = ecs.createEntity();
	auto entity_2 = ecs.createEntity();

	ASSERT_THROW(ecs.hasComponent<Speed>(entity_1), std::out_of_range);
	ASSERT_THROW(ecs.hasComponent<Position>(entity_2), std::out_of_range);

	ASSERT_THROW(ecs.getComponent<Speed>(entity_1), std::out_of_range);
	ASSERT_THROW(ecs.getComponent<Position>(entity_2), std::out_of_range);

	ASSERT_THROW(const_ecs.getComponent<Speed>(entity_1), std::out_of_range);
	ASSERT_THROW(const_ecs.getComponent<Position>(entity_2), std::out_of_range);

	ASSERT_THROW(ecs.addComponent<Speed>(entity_1), std::out_of_range);
	ASSERT_THROW(ecs.addComponent<Position>(entity_2, 1, 1), std::out_of_range);


	std::cout<<std::boolalpha<<std::is_constructible_v<Position, int>;
	ecs.registerComponent<Speed>();
	ecs.registerComponent<Position>();
	ecs.registerComponent<Acceleration>();

	ASSERT_FALSE(ecs.hasComponent<Speed>(entity_1));
	ASSERT_FALSE(ecs.hasComponent<Position>(entity_2));
	ASSERT_FALSE(ecs.hasComponent<Acceleration>(entity_1));

	//by constructor
	ecs.addComponent<Speed>(entity_1, 4);
	ecs.addComponent<Position>(entity_1, 1);
	ecs.addComponent<Acceleration>(entity_2, 1, 2, 3);


	ASSERT_FALSE(ecs.hasComponent<Speed>(entity_2));
	ASSERT_FALSE(ecs.hasComponent<Position>(entity_2));
	ASSERT_FALSE(ecs.hasComponent<Acceleration>(entity_1));

	ASSERT_TRUE(ecs.hasComponent<Speed>(entity_1));
	ASSERT_TRUE(ecs.hasComponent<Position>(entity_1));
	ASSERT_TRUE(ecs.hasComponent<Acceleration>(entity_2));

	ASSERT_EQ(Speed{4}, ecs.getComponent<Speed>(entity_1));
	ASSERT_EQ(Position{1}, ecs.getComponent<Position>(entity_1));
	ASSERT_EQ(Acceleration(1, 2, 3), ecs.getComponent<Acceleration>(entity_2));

	ecs.removeComponent<Speed>(entity_1);
	ecs.removeComponent<Position>(entity_1);
	ecs.removeComponent<Acceleration>(entity_2);

	ASSERT_FALSE(ecs.hasComponent<Speed>(entity_1));
	ASSERT_FALSE(ecs.hasComponent<Position>(entity_1));
	ASSERT_FALSE(ecs.hasComponent<Acceleration>(entity_2));

	//by value/reference
	const Acceleration accel{1, 3, 3};
	ecs.addComponent<Position>(entity_1, Position{4});
	ecs.addComponent<Acceleration>(entity_2, accel);

	ASSERT_EQ(Position{4}, ecs.getComponent<Position>(entity_1));
	ASSERT_EQ(Acceleration(1, 3, 3), ecs.getComponent<Acceleration>(entity_2));
}
