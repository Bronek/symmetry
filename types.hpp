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
}


