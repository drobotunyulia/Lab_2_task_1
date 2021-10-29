#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <functional>
#include <vector>
#include "comparator.h"
#include "node.h"
#include "treeexception.h"
#include "templatemethod.h"



template <typename TKey, typename TValue>
class binary_tree
{
public:
    //"необходимый элемент отсутствует"
    class invalid_key_exception : public tree_exception
    {
    public:
        invalid_key_exception(): tree_exception("Invalid key...")
        {
        }
    };
    //"элемент уже имеется"
    class item_exists_exception : public tree_exception
    {
    public:
        item_exists_exception() : tree_exception("Item already exists...")
        {
        }
    };
    typedef std::function<void(TKey key, TValue value, int depth)> callback_function;

    binary_tree(find_template_method<TKey, TValue> finder,
                insert_template_method<TKey, TValue> inserter,
                remove_template_method<TKey, TValue> remover,
                comparator<TKey> &key_comparator);
    binary_tree(binary_tree<TKey, TValue> &tree);
    ~binary_tree();
    binary_tree& operator = (const binary_tree &tree_object);

    TValue find(TKey key);
    void insert(TKey key, TValue value);
    void remove(TKey key);

    void prefix_traversal(callback_function function);
    void postfix_traversal(callback_function function);
    void infix_traversal(callback_function function);
protected:
    void prefix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth);
    void postfix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth);
    void infix_traversal_base(node<TKey, TValue> *roott_node, callback_function function, int depth);
private:
    node<TKey, TValue> *root_node = nullptr;
    comparator<TKey> key_comparator;
    find_template_method<TKey, TValue> finder;
    insert_template_method<TKey, TValue> inserter;
    remove_template_method<TKey, TValue> remover;

};

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(find_template_method<TKey, TValue> finder,
            insert_template_method<TKey, TValue> inserter,
            remove_template_method<TKey, TValue> remover,
            comparator<TKey> &key_comparator)
{
    this->finder = finder;
    this->inserter = inserter;
    this->remover = remover;
    this->key_comparator = key_comparator;
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(binary_tree<TKey, TValue> &tree)
//конструктор копирования
{
    status_t status;
    this->finder = tree.finder;
    this->inserter = tree.inserter;
    this->remover = tree.remover;
    this->key_comparator = tree.key_comparator;
    std::vector<node<TKey, TValue>> nodes;
    prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->inserter.invoke_insert(this->root_node, nodes[i].key, nodes[i].value, this->key_comparator, status);
    }
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::~binary_tree()
{
    status_t status;
    std::vector<node<TKey, TValue>> nodes;
    prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->remover.invoke_remove(this->root_node, nodes[i].key, this->key_comparator, status);
    }
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>& binary_tree<TKey, TValue>::operator = (const binary_tree &tree)
{
    status_t status;
    this->finder = tree.finder;
    this->inserter = tree.inserter;
    this->remover = tree.remover;
    this->key_comparator = tree.key_comparator;
    std::vector<node<TKey, TValue>> nodes;
    prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->inserter.invoke_insert(this->root_node, nodes[i].key, nodes[i].value, this->key_comparator, status);
    }
    return *this;
}

template <typename TKey, typename TValue>
TValue binary_tree<TKey, TValue>::find(TKey key)
{
    status_t status;
    node<TKey, TValue> *find_node = finder.invoke_find(this->root_node, key, this->key_comparator, status);
    if (status == FIND_ERROR)
    {
        throw item_exists_exception();
    }
    return find_node->value;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert(TKey key, TValue value)
{
    status_t status;
    inserter.invoke_insert(this->root_node, key, value, this->key_comparator, status);
    if (status == INSERT_ERROR)
    {
        throw invalid_key_exception();
    }
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove(TKey key)
{
    status_t status;
    remover.invoke_remove(this->root_node, key, this->key_comparator, status);
    if (status == REMOVE_ERROR)
    {
        throw invalid_key_exception();
    }
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::prefix_traversal(callback_function function)
{
    prefix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::postfix_traversal(callback_function function)
{
    postfix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::infix_traversal(callback_function function)
{
    infix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::prefix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth)
{
    if (root_node)
    {
        function(root_node->key, root_node->value, depth);
        if (root_node->left)
        {
            prefix_traversal_base(root_node->left, function, depth + 1);
        }
        if (root_node->right)
        {
            prefix_traversal_base(root_node->right, function, depth + 1);
        }
    }
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::postfix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth)
{
    if (root_node)
    {
        if (root_node->left)
        {
            postfix_traversal_base(root_node->left, function, depth + 1);
        }
        if (root_node->left)
        {
            postfix_traversal_base(root_node->right, function, depth + 1);
        }
        function(root_node->key, root_node->value, depth);
    }
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::infix_traversal_base(node<TKey, TValue> *roott_node, callback_function function, int depth)
{
    if (roott_node)
    {
        if (roott_node->left)
        {
            infix_traversal_base(roott_node->left, function, depth + 1);
        }
        function(roott_node->key, roott_node->value, depth);
        if (roott_node->right)
        {
            infix_traversal_base(roott_node->right, function, depth + 1);
        }
    }
}

#endif // BINARYTREE_H
