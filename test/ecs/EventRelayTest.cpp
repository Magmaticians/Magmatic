#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ecs/EventRelay.hpp"

using namespace magmatic::ecs;

class EventRelayTest : public ::testing::Test
{
protected:
	EventRelay relay;
};


TEST_F(EventRelayTest, sending_receiving_event_by_value)
{
	struct OneValue
	{
		int val1;
	};
	OneValue test_value(1);

	testing::MockFunction<void(const OneValue&)> one_value_callback1;

	relay.subscribe<OneValue>(one_value_callback1.AsStdFunction());

	EXPECT_CALL(one_value_callback1, Call(testing::Ref(test_value)))
	.Times(1);
	relay.emit<OneValue>(test_value);
}

TEST_F(EventRelayTest, sending_receiving_event_more_receivers)
{
	struct OneValue
	{
		int val1;
	};

	OneValue test_value(3);

	testing::MockFunction<void(const OneValue&)> one_value_callback1;
	testing::MockFunction<void(const OneValue&)> one_value_callback2;

	relay.subscribe<OneValue>(one_value_callback1.AsStdFunction());
	relay.subscribe<OneValue>(one_value_callback2.AsStdFunction());

	EXPECT_CALL(one_value_callback1, Call(testing::Ref(test_value)))
			.Times(1);
	EXPECT_CALL(one_value_callback2, Call(testing::Ref(test_value)))
			.Times(1);
	relay.emit<OneValue>(test_value);
}

TEST_F(EventRelayTest, unsubscribe)
{
	struct OneValue
	{
		int val1;
	};

	OneValue test_value(3);

	testing::MockFunction<void(const OneValue&)> one_value_callback1;
	testing::MockFunction<void(const OneValue&)> one_value_callback2;

	relay.subscribe<OneValue>(one_value_callback1.AsStdFunction());
	auto id = relay.subscribe<OneValue>(one_value_callback2.AsStdFunction());
	relay.unsubscribe<OneValue>(id);

	EXPECT_CALL(one_value_callback1, Call(testing::Ref(test_value)))
			.Times(1);
	EXPECT_CALL(one_value_callback2, Call(testing::Ref(test_value)))
			.Times(0);
	relay.emit<OneValue>(test_value);

	EXPECT_DEATH(relay.unsubscribe<OneValue>(id), "");
}

TEST_F(EventRelayTest, subscribers_count)
{
	struct OneValue
	{
		int val1;
	};

	struct AnotherOneValue
	{
		int value1;
	};

	testing::MockFunction<void(const OneValue&)> one_value_callback1;
	testing::MockFunction<void(const OneValue&)> one_value_callback2;

	relay.subscribe<OneValue>(one_value_callback1.AsStdFunction());
	ASSERT_EQ(1, relay.subscribers_count<OneValue>());

	const auto id = relay.subscribe<OneValue>(one_value_callback2.AsStdFunction());
	ASSERT_EQ(2, relay.subscribers_count<OneValue>());

	relay.unsubscribe<OneValue>(id);
	ASSERT_EQ(1, relay.subscribers_count<OneValue>());

	ASSERT_EQ(0, relay.subscribers_count<AnotherOneValue>());
}

TEST_F(EventRelayTest, sending_receiving_event_by_default_constrcutible)
{
	constexpr int default_value = 7;
	struct OneValue
	{
		OneValue(): val1(default_value) {};
		int val1;
	};

	testing::MockFunction<void(const OneValue&)> one_value_callback1;
	testing::MockFunction<void(const OneValue&)> one_value_callback2;

	relay.subscribe<OneValue>(one_value_callback1.AsStdFunction());
	relay.subscribe<OneValue>(one_value_callback2.AsStdFunction());

	EXPECT_CALL(one_value_callback1, Call(testing::Field(&OneValue::val1, default_value)))
			.Times(1);
	EXPECT_CALL(one_value_callback2, Call(testing::Field(&OneValue::val1, default_value)))
			.Times(1);
	relay.emit<OneValue>();
}

MATCHER_P(TwoValuesAreEqual, other,"")
{
	return arg.val1 == other.val1 && arg.val2 == other.val2;
}

TEST_F(EventRelayTest, sending_receiving_event_by_argument_forwarding)
{
	struct TwoValues
	{
		int val1;
		int val2;
	};

	testing::MockFunction<void(const TwoValues&)> two_values_callback1;
	testing::MockFunction<void(const TwoValues&)> two_values_callback2;

	relay.subscribe<TwoValues>(two_values_callback1.AsStdFunction());
	relay.subscribe<TwoValues>(two_values_callback2.AsStdFunction());

	TwoValues two_values(1, 2);

	EXPECT_CALL(two_values_callback1, Call(TwoValuesAreEqual(two_values)))
			.Times(1);
	EXPECT_CALL(two_values_callback2, Call(TwoValuesAreEqual(two_values)))
			.Times(1);
	relay.emit<TwoValues>(1, 2);
}

TEST_F(EventRelayTest, sending_receiving_event_different_types)
{
	struct OneValue
	{
		int val1;
	};

	struct TwoValues
	{
		int val1;
		int val2;
	};

	testing::MockFunction<void(const TwoValues&)> two_values_callback1;
	testing::MockFunction<void(const TwoValues&)> two_values_callback2;
	testing::MockFunction<void(const OneValue&)> one_value_callback1;

	relay.subscribe<TwoValues>(two_values_callback1.AsStdFunction());
	relay.subscribe<TwoValues>(two_values_callback2.AsStdFunction());
	relay.subscribe<OneValue>(one_value_callback1.AsStdFunction());

	TwoValues two_values(1, 2);
	OneValue one_value(5);

	EXPECT_CALL(two_values_callback1, Call(testing::Ref(two_values)))
			.Times(1);
	EXPECT_CALL(two_values_callback2, Call(testing::Ref(two_values)))
			.Times(1);
	EXPECT_CALL(one_value_callback1, Call(testing::Ref(one_value)))
			.Times(1);
	relay.emit<TwoValues>(two_values);
	relay.emit<OneValue>(one_value);
}

