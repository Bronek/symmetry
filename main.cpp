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
