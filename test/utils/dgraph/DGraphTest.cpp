#include <gtest/gtest.h>
#include "utils/dgraph/DGraph.hpp"


class DGraphTest : public ::testing::Test
{
protected:
	DGraph<int> graph;
};

class DGraphIteratorTest : public ::testing::Test
{
protected:
	DGraph<int> graph;
	const DGraph<int>& c_graph = graph;

	void SetUp() override
	{
		graph.push_back(2);
		graph.push_back(4);
		graph.push_back(3);
	}
};

TEST_F(DGraphTest, empty)
{
	ASSERT_TRUE(graph.empty());

	graph.push_back(8);

	ASSERT_FALSE(graph.empty());
}

TEST_F(DGraphTest, pushingElementsOnEnd)
{
	const int value = 2;

	ASSERT_EQ(0, graph.size());
	graph.push_back(value);
	graph.push_back(2);
	ASSERT_EQ(2, graph.size());
	graph.push_back(2);
	ASSERT_EQ(3, graph.size());
}

TEST_F(DGraphTest, accessingElementsUnchecked)
{
	graph.push_back(3);
	graph.push_back(2);

	const auto node_1 = graph[0];
	const auto node_2 = graph[1];

	ASSERT_EQ(0, node_1.id());
	ASSERT_EQ(1, node_2.id());
	ASSERT_EQ(3, *node_1);
	ASSERT_EQ(2, *node_2);

	const auto& const_graph = graph;

	const auto node_3 = const_graph[0];
	const auto node_4 = const_graph[1];

	ASSERT_EQ(0, node_3.id());
	ASSERT_EQ(1, node_4.id());
	ASSERT_EQ(3, *node_3);
	ASSERT_EQ(2, *node_4);
}

TEST_F(DGraphTest, accessingElementsChecked)
{
	graph.push_back(3);
	graph.push_back(2);

	const auto node_1 = graph.at(0);
	const auto node_2 = graph.at(1);

	ASSERT_EQ(0, node_1.id());
	ASSERT_EQ(1, node_2.id());
	ASSERT_EQ(3, *node_1);
	ASSERT_EQ(2, *node_2);

	const auto& const_graph = graph;

	const auto node_3 = const_graph.at(0);
	const auto node_4 = const_graph.at(1);

	ASSERT_EQ(0, node_3.id());
	ASSERT_EQ(1, node_4.id());
	ASSERT_EQ(3, *node_3);
	ASSERT_EQ(2, *node_4);

	ASSERT_THROW(graph.at(3), std::out_of_range);
	ASSERT_THROW(const_graph.at(3), std::out_of_range);
}

TEST_F(DGraphTest, iteratorBeginEnd)
{
	graph.push_back(0);
	graph.push_back(1);
	graph.push_back(2);

	const auto& c_graph = graph;

	auto begin = graph.begin();
	auto cbegin = graph.cbegin();
	auto c_begin = c_graph.begin();

	ASSERT_EQ(begin, cbegin);
	ASSERT_EQ(cbegin, c_begin);

	ASSERT_EQ(0, (*begin).id());
	ASSERT_EQ(0, (*cbegin).id());
	ASSERT_EQ(0, (*c_begin).id());

	auto end = graph.end();
	auto cend = graph.cend();
	auto c_end = c_graph.end();

	ASSERT_EQ(end, cend);
	ASSERT_EQ(cend, c_end);

	ASSERT_EQ(3, std::distance(begin, end));
	ASSERT_EQ(3, std::distance(cbegin, cend));
	ASSERT_EQ(3, std::distance(c_begin, c_end));
}

TEST_F(DGraphIteratorTest, iteratorDereferenceAdvance)
{
	auto iter = graph.begin();
	auto citer = c_graph.begin();

	ASSERT_EQ(0, (*iter).id());
	ASSERT_EQ(0, (*citer).id());

	ASSERT_EQ(1, (*++iter).id());
	ASSERT_EQ(1, (*++citer).id());

	ASSERT_EQ(1, (*iter++).id());
	ASSERT_EQ(1, (*citer++).id());

	ASSERT_EQ(2, (*iter--).id());
	ASSERT_EQ(2, (*citer--).id());

	ASSERT_EQ(0, (*--iter).id());
	ASSERT_EQ(0, (*--citer).id());

	ASSERT_EQ(0, (*iter).id());
	ASSERT_EQ(0, (*citer).id());
}

TEST_F(DGraphIteratorTest, equality)
{

	auto iter = graph.begin();
	auto citer = c_graph.begin();

	ASSERT_TRUE(iter == citer);
	ASSERT_TRUE(iter == iter);
	ASSERT_TRUE(iter == citer);

	ASSERT_FALSE(iter != citer);
	ASSERT_FALSE(iter != iter);
	ASSERT_FALSE(citer != citer);

	auto iter2 = iter++;
	auto citer2 = citer++;

	ASSERT_FALSE(iter == iter2);
	ASSERT_FALSE(citer == citer2);
	ASSERT_FALSE(iter == citer2);

	ASSERT_TRUE(iter2 == iter2);
	ASSERT_TRUE(citer2 == citer2);
	ASSERT_TRUE(citer2 == iter2);
}

TEST_F(DGraphIteratorTest, comparison)
{
	auto iter = graph.begin();
	auto citer = c_graph.begin();

	ASSERT_TRUE(iter <= iter);
	ASSERT_TRUE(iter <= citer);
	ASSERT_TRUE(citer <= iter);
	ASSERT_TRUE(citer <= citer);

	ASSERT_TRUE(iter >= iter);
	ASSERT_TRUE(iter >= citer);
	ASSERT_TRUE(citer >= iter);
	ASSERT_TRUE(citer >= citer);

	ASSERT_FALSE(iter < iter);
	ASSERT_FALSE(citer < citer);
	ASSERT_FALSE(iter < citer);
	ASSERT_FALSE(citer < iter);

	ASSERT_FALSE(iter > iter);
	ASSERT_FALSE(citer > citer);
	ASSERT_FALSE(iter > citer);
	ASSERT_FALSE(citer > iter);

	auto iter2 = iter++;
	auto citer2 = citer++;

	ASSERT_TRUE(iter < iter2);
	ASSERT_TRUE(iter < citer2);
	ASSERT_TRUE(citer < iter2);
	ASSERT_TRUE(citer < citer2);

	ASSERT_FALSE(iter > iter2);
	ASSERT_FALSE(iter > citer2);
	ASSERT_FALSE(citer > iter2);
	ASSERT_FALSE(citer > citer2);

	ASSERT_TRUE(iter <= iter2);
	ASSERT_TRUE(iter <= citer2);
	ASSERT_TRUE(citer <= iter2);
	ASSERT_TRUE(citer <= citer2);

	ASSERT_FALSE(iter2 <= iter);
	ASSERT_FALSE(citer2 <= iter);
	ASSERT_FALSE(iter2 <= citer);
	ASSERT_FALSE(citer2 <= citer);

	ASSERT_TRUE(iter2 >= iter);
	ASSERT_TRUE(citer2 >= iter);
	ASSERT_TRUE(iter2 >= citer);
	ASSERT_TRUE(citer2 >= citer);

	ASSERT_FALSE(iter >= iter2);
	ASSERT_FALSE(iter >= citer2);
	ASSERT_FALSE(citer >= iter2);
	ASSERT_FALSE(citer >= citer2);
}

TEST_F(DGraphIteratorTest, threeWayComparison)
{
	auto iter = graph.begin();
	auto citer = c_graph.begin();

	auto iter2 = iter++;
	auto citer2 = citer++;

	ASSERT_EQ(std::strong_ordering::equal, iter <=> citer);
	ASSERT_EQ(std::strong_ordering::equal, iter <=> iter);
	ASSERT_EQ(std::strong_ordering::less, iter <=> iter2);
	ASSERT_EQ(std::strong_ordering::greater, citer2 <=> iter);
}

TEST_F(DGraphIteratorTest, randomAccess)
{
	auto iter = graph.begin();
	auto citer = c_graph.begin();

	const auto iter1 = iter+2;
	const auto citer1 = citer + 1;

	auto iter2 = iter1 - 1;
	auto citer2 = citer1 - 1;

	ASSERT_EQ(2, (*iter1).id());
	ASSERT_EQ(1, (*citer1).id());
	ASSERT_EQ(1, (*iter2).id());
	ASSERT_EQ(0, (*citer2).id());

	iter2 += 1;
	citer2 += 1;

	ASSERT_EQ(2, (*iter2).id());
	ASSERT_EQ(1, (*citer2).id());

	iter2 -= 2;
	citer2 -= 1;

	ASSERT_EQ(0, (*iter2).id());
	ASSERT_EQ(0, (*citer2).id());
}

TEST_F(DGraphTest, popBack)
{
	graph.push_back(7);
	graph.push_back(3);

	graph.pop_back();

	ASSERT_EQ(0, graph[0].id());
	ASSERT_THROW(graph.at(1), std::out_of_range);
}