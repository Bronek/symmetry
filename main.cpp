#include <gtest/gtest.h>

#include <cstdio>
#include <cstring>
#include <sstream>
#include <memory>

#include "types.hpp"

TEST(test_tree, simple_construction) {
    using namespace sym;
    std::shared_ptr<node<int>> p;
    p.reset(new node<int>(nullptr, 0));
    EXPECT_EQ(p->parent, nullptr);
    EXPECT_EQ(p->value, 0);
    EXPECT_EQ(p->child[0], nullptr);
    EXPECT_EQ(p->child[1], nullptr);

    std::shared_ptr<node<int>> p0;
    p0.reset(new node<int>(p.get(), 0));
    std::shared_ptr<node<int>> p1;
    p1.reset(new node<int>(p.get(), 1));
    p->child[0] = p0.get();
    p->child[1] = p1.get();

    EXPECT_EQ(p->child[0], p0.get());
    EXPECT_EQ(p->child[1], p1.get());
    EXPECT_EQ(p0->parent, p.get());
    EXPECT_EQ(p0->value, 0);
    EXPECT_EQ(p0->child[0], nullptr);
    EXPECT_EQ(p0->child[1], nullptr);
    EXPECT_EQ(p1->parent, p.get());
    EXPECT_EQ(p1->value, 1);
    EXPECT_EQ(p1->child[0], nullptr);
    EXPECT_EQ(p1->child[1], nullptr);
}

TEST(test_tree, tree_append)
{
    using namespace sym;
    tree<int> t {42};
    EXPECT_EQ(t.head, t.data.back().get());
    EXPECT_EQ(t.head->child[0], nullptr);
    EXPECT_EQ(t.head->child[1], nullptr);
    EXPECT_EQ(t.head->value, 42);
    EXPECT_EQ(t.data.size(), 1);

    const auto n0 = t.append(t.head, 0, 41);
    EXPECT_EQ(t.head->child[0], n0);
    EXPECT_EQ(t.head->child[1], nullptr);
    EXPECT_EQ(n0, t.data.back().get());
    EXPECT_EQ(n0->value, 41);
    EXPECT_EQ(t.data.size(), 2);

    const auto n1 = t.append(t.head, 1, 41);
    EXPECT_EQ(t.head->child[0], n0);
    EXPECT_EQ(t.head->child[1], n1);
    EXPECT_EQ(n1->value, 41);
    EXPECT_EQ(t.data.size(), 3);

    auto pp1 = std::make_pair(n0, n1);
    auto pp2 = t.append(pp1, 0, 40);
    EXPECT_EQ(n0->child[0], pp2.first);
    EXPECT_EQ(n0->child[1], nullptr);
    EXPECT_EQ(n1->child[0], nullptr);
    EXPECT_EQ(n1->child[1], pp2.second);
    EXPECT_EQ(n0->child[0]->value, 40);
    EXPECT_EQ(n1->child[1]->value, 40);
    EXPECT_EQ(t.data.size(), 5);

    auto pp3 = t.append(pp1, 1, 39);
    EXPECT_EQ(n0->child[0], pp2.first);
    EXPECT_EQ(n0->child[1], pp3.first);
    EXPECT_EQ(n1->child[0], pp3.second);
    EXPECT_EQ(n1->child[1], pp2.second);
    EXPECT_EQ(n0->child[0]->value, 40);
    EXPECT_EQ(n0->child[1]->value, 39);
    EXPECT_EQ(n1->child[0]->value, 39);
    EXPECT_EQ(n1->child[1]->value, 40);
    EXPECT_EQ(t.data.size(), 7);

    auto pp4 = t.append(pp3, 0, 38);
    EXPECT_EQ(n0->child[1]->child[0], pp4.first);
    EXPECT_EQ(n1->child[0]->child[1], pp4.second);
    EXPECT_EQ(t.data.size(), 9);
}

struct test_algo : ::testing::Test {
    sym::tree<int> t {42};
    std::pair<sym::node<int>*, sym::node<int>*> pp[5];

    test_algo()
    {
        pp[0].first = t.append(t.head, 0, 41);
        pp[0].second = t.append(t.head, 1, 41);
        pp[1] = t.append(pp[0], 0, 40);
        pp[2] = t.append(pp[0], 1, 39);
        pp[3] = t.append(pp[2], 0, 38);
    }
};

TEST_F(test_algo, recursive)
{
    using namespace sym;

    EXPECT_FALSE(symmr(t.head, pp[0].first));
    EXPECT_FALSE(symmr(t.head, pp[0].second));
    EXPECT_TRUE(symmr(pp[0].first, pp[0].second));
    EXPECT_TRUE(symmr(pp[0].second, pp[0].first));
    EXPECT_TRUE(symmr(pp[1].first, pp[1].second));
    EXPECT_TRUE(symmr(pp[1].second, pp[1].first));
    EXPECT_TRUE(symmr(pp[2].first, pp[2].second));
    EXPECT_TRUE(symmr(pp[2].second, pp[2].first));
    EXPECT_TRUE(symmr(pp[3].first, pp[3].second));
    EXPECT_TRUE(symmr(pp[3].second, pp[3].first));
    EXPECT_FALSE(symmr(pp[0].first, pp[2].second));
    EXPECT_FALSE(symmr(pp[1].second, pp[3].first));
    EXPECT_FALSE(symmr(pp[1].first, pp[2].second));
    EXPECT_FALSE(symmr(pp[3].second, pp[0].first));
    EXPECT_FALSE(symmr(t.head, pp[3].second));
    EXPECT_FALSE(symmr(t.head, pp[2].first));
    EXPECT_FALSE(symmr(t.head, pp[1].second));
    EXPECT_FALSE(symmr(t.head, pp[0].first));
    EXPECT_TRUE(symmr(t.head, t.head));
    EXPECT_TRUE(symmr(t));

    // Break symmetry
    t.append(pp[3].first, 0, 13);
    EXPECT_FALSE(symmr(t));
}

TEST_F(test_algo, iterative)
{
    using namespace sym;

    EXPECT_TRUE(symmi(t));

    // Break symmetry
    t.append(pp[3].first, 0, 13);
    EXPECT_FALSE(symmi(t));

    // Fix symmetry
    t.append(pp[3].second, 1, 13);
    EXPECT_TRUE(symmi(t));

    // A tree with head only is by definition symmetrical
    tree<int> t1 {16};
    EXPECT_TRUE(symmi(t1));
    // Break symmetry
    t1.append(t1.head, 0, 15);
    EXPECT_FALSE(symmi(t1));
    // Add second child, symmetric topology but not symmetric value
    auto p1 = t1.append(t1.head, 1, 14);
    EXPECT_FALSE(symmi(t1));
    // Update value of second child to make it symmetric
    p1->value = 15;
    EXPECT_TRUE(symmi(t1));
}
