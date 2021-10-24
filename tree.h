#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <functional>
#include <vector>
#include "comparator.h"
#include "node.h"
#include "treexception.h"

enum status {
    FIND_LEFT,
    FIND_RIGHT,
    FIND_ROOT,
    INSERT_LEFT,
    INSERT_RIGHT,
    INSERT_ROOT,
    REMOVE_LEFT,
    REMOVE_RIGHT,
    REMOVE_ROOT
} typedef status_t;

template <typename TKey, typename TValue>
class tree
{
public:
    class invalid_key_exception : public tree_exception
    {
    public:
        invalid_key_exception();
    };
    class item_exists_exception : public tree_exception
    {
    public:
        item_exists_exception();
    };
    typedef std::function<void(TKey key, TValue value, int depth)> callback_function;
    tree();
    tree(comparator<TKey> &key_comparator);
    tree(TKey root_key, TValue root_value, comparator<TKey> &key_comparator);
    tree(tree<TKey, TValue> &tree);
    ~tree();
    tree& operator = (const tree &tree_object);
    void insert_node(TKey key, TValue value);
    node<TKey, TValue> *find_node(TKey key);
    TValue find_value(TKey key);
    void remove_node(TKey key);
    void prefix_traversal(callback_function function);
    void postfix_traversal(callback_function function);
    void infix_traversal(callback_function function);
protected:
    node<TKey, TValue> *root_node = nullptr;
    comparator<TKey> key_comparator;
    virtual node<TKey, TValue> *insert_node_base(node<TKey, TValue> *root_node, TKey key, TValue value, status_t &status) = 0;
    virtual node<TKey, TValue> *find_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status) = 0;
    virtual node<TKey, TValue> *remove_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status) = 0;
    void prefix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth);
    void postfix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth);
    void infix_traversal_base(node<TKey, TValue> *roott_node, callback_function function, int depth);
    node<TKey, TValue> *find_min_node(node<TKey, TValue>* root_node);
    virtual void insert_node_hook_prev();
    virtual void insert_node_hook_post(int status);
    virtual void remove_node_hook_prev();
    virtual void remove_node_hook_post(int status);
    virtual void find_node_hook_prev();
    virtual void find_node_hook_post(int status);
};

template <typename TKey, typename TValue>
tree<TKey, TValue>::tree()
{

}

template <typename TKey, typename TValue>
tree<TKey, TValue>::tree(comparator<TKey> &key_comparator)
{
    this->key_comparator = key_comparator;
}

template <typename TKey, typename TValue>
tree<TKey, TValue>::tree(TKey root_key, TValue root_value, comparator<TKey> &key_comparator)
{
    this->key_comparator = key_comparator;
    this->root_node = new node<TKey, TValue>(root_key, root_value);
}

template <typename TKey, typename TValue>
tree<TKey, TValue>::tree(tree<TKey, TValue> &tree)
{
    key_comparator = tree.key_comparator;
    std::vector<node<TKey, TValue>> nodes;
    tree.prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->insert_node(nodes[i]);
    }
}

template <typename TKey, typename TValue>
tree<TKey, TValue>::~tree()
{
    std::vector<node<TKey, TValue>> nodes;
    prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->remove_node(nodes[i].key);
    }
}

template <typename TKey, typename TValue>
tree<TKey, TValue>& tree<TKey, TValue>::operator = (const tree &tree_object)
{
    key_comparator = tree_object.key_comparator;
    std::vector<node<TKey, TValue>> nodes;
    tree_object.prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->insert_node(nodes[i]);
    }
    return *this;
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::insert_node(TKey key, TValue value)
{
    status_t status;
    insert_node_hook_prev();
    root_node = insert_node_base(root_node, key, value, status);
    insert_node_hook_post(status);
}

template <typename TKey, typename TValue>
node<TKey, TValue> *tree<TKey, TValue>::find_node(TKey key)
{
    status_t status;
    node<TKey, TValue> *result_node = new node<TKey, TValue>();
    find_node_hook_prev();
    result_node = find_node_base(root_node, key, status);
    find_node_hook_post(status);
    return result_node;
}

template <typename TKey, typename TValue>
TValue tree<TKey, TValue>::find_value(TKey key)
{
    return find_node(key)->value;
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::remove_node(TKey key)
{
    status_t status;
    remove_node_hook_prev();
    root_node = remove_node_base(root_node, key, status);
    remove_node_hook_post(status);
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::prefix_traversal(callback_function function)
{
    prefix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::postfix_traversal(callback_function function)
{
    postfix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::infix_traversal(callback_function function)
{
    infix_traversal_base(root_node, function, 0);
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::prefix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth)
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
void tree<TKey, TValue>::postfix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth)
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
void tree<TKey, TValue>::infix_traversal_base(node<TKey, TValue> *roott_node, callback_function function, int depth)
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

template <typename TKey, typename TValue>
node<TKey, TValue>* tree<TKey, TValue>::find_min_node(node<TKey, TValue>* root_node)
{
    if(!root_node)
        return root_node;
    else if(!root_node->left)
        return root_node;
    else
        return find_min_node(root_node->left);
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::insert_node_hook_prev()
{
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::insert_node_hook_post(int status)
{
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::remove_node_hook_prev()
{
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::remove_node_hook_post(int status)
{
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::find_node_hook_prev()
{
}

template <typename TKey, typename TValue>
void tree<TKey, TValue>::find_node_hook_post(int status)
{
}

template <typename TKey, typename TValue>
tree<TKey, TValue>::invalid_key_exception::invalid_key_exception() : tree_exception("Invalid key...")
{
}

template <typename TKey, typename TValue>
tree<TKey, TValue>::item_exists_exception::item_exists_exception() : tree_exception("Item already exists...")
{
}

#endif // TREE_H
