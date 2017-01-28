#pragma once

namespace sym {
    template <typename Type> struct node
    {
        constexpr node(const node* p, const Type& v)
            : parent(p), child{nullptr, nullptr}, value(v)
        { }

        const node* const parent;
        node* child[2];
        Type value;
    };

    template <typename Type> struct tree
    {
        node<Type>* const head;
        std::vector<std::unique_ptr<node<Type>>> data;

        explicit tree(const Type& v) : head(new node<Type>(nullptr, v))
        {
            data.emplace_back(head);
        }

        // Append a single child to node p
        node<Type>* append(node<Type>* p, int i, const Type& v)
        {
            assert(i >= 0 && i < 2);
            assert(p->child[i] == nullptr);
            data.emplace_back(new node<Type>(p, v));
            p->child[i] = data.back().get();
            return p->child[i];
        }

        // Append a single child to each nodes, in a symmetrical manner. Note, two
        // copies of same pointers wrapped in a pair is valid input.
        std::pair<node<Type>*, node<Type>*> append(std::pair<node<Type>*, node<Type>*> pp, int i, const Type& v)
        {
            assert(i >= 0 && i < 2);
            assert(pp.first->child[i] == nullptr);
            assert(pp.second->child[1 - i] == nullptr);
            data.emplace_back(new node<Type>(pp.first, v));
            pp.first->child[i] = data.back().get();
            data.emplace_back(new node<Type>(pp.second, v));
            pp.second->child[1 - i] = data.back().get();
            return std::make_pair(pp.first->child[i], pp.second->child[1 - i]);
        }
    };

    // Recursive check if two nodes are symmetrical to each other
    template <typename Type> bool symmr(const node<Type>* l, const node<Type>* r)
    {
        return ((l->child[0] == nullptr) == (r->child[1] == nullptr))
               && ((l->child[1] == nullptr) == (r->child[0] == nullptr))
               && l->value == r->value
               && ((l->child[0] == nullptr) || symmr(l->child[0], r->child[1]))
               && ((l->child[1] == nullptr) || symmr(l->child[1], r->child[0]));
    }

    // Wrapper to run recursive check on the whole tree
    template <typename Type> bool symmr(const tree<Type>& t)
    {
        return symmr(t.head, t.head);
    }

    // Iterative check if two children are symmetrical to each other
    template <typename Type> bool symmi(const tree<Type>& t)
    {
        // This last node will be always tracking next[0] node only
        const node<Type>* last = nullptr;
        const node<Type>* next[2] = {t.head, t.head};
        while (((next[0]->child[0] == nullptr) == (next[1]->child[1] == nullptr))
                && ((next[0]->child[1] == nullptr) == (next[1]->child[0] == nullptr))
                && next[0]->value == next[1]->value)
        {
            // Figure out the next node to visit
            if (next[0]->child[0] != nullptr
                && last != next[0]->child[0]
                && last != next[0]->child[1])
            {
                // Left node is available and we are not backing up the tree from either left or right side
                last = next[0];
                next[0] = next[0]->child[0];
                next[1] = next[1]->child[1];
            }
            else if (next[0]->child[1] != nullptr
                     && last != next[0]->child[1])
            {
                // Right node is available and we are not backing up the tree from right side
                last = next[0];
                next[0] = next[0]->child[1];
                next[1] = next[1]->child[0];
            }
            else
            {
                // We are backing up the tree ...
                if (next[0] == t.head)
                    return true; // ... but we already visited all the nodes
                last = next[0];
                next[0] = next[0]->parent;
                next[1] = next[1]->parent;
            }
        }

        return false;
    }
}
