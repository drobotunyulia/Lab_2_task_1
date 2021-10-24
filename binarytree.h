#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <exception>
#include <iostream>
#include <vector>
#include <functional>

#include "tree.h"

template <typename TKey, typename TValue>
class binary_tree : public tree<TKey, TValue>
{
public:
    binary_tree();
    binary_tree(comparator<TKey> &key_comparator);
    binary_tree(TKey root_key, TValue root_value, comparator<TKey> &key_comparator);
    binary_tree(binary_tree<TKey, TValue> &tree_object);
    ~binary_tree();
protected:
    node<TKey, TValue> *root_node = nullptr;
    comparator<TKey> key_comparator;
    node<TKey, TValue> *insert_node_base(node<TKey, TValue> *root_node, TKey key, TValue value, status_t &status) override;
    node<TKey, TValue> *find_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status) override;
    node<TKey, TValue> *remove_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status) override;
};

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree()
{
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(comparator<TKey> &key_comparator) : tree<TKey, TValue>::tree(key_comparator)
{
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(TKey root_key, TValue root_value, comparator<TKey> &key_comparator) : tree<TKey, TValue>::tree(root_key, root_value, key_comparator)
{
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(binary_tree<TKey, TValue> &tree_object) : tree<TKey, TValue>::tree(tree_object)
{
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::~binary_tree()
{
}

template <typename TKey, typename TValue>
node<TKey, TValue> *binary_tree<TKey, TValue>::insert_node_base(node<TKey, TValue> *root_node, TKey key, TValue value, status_t &status)
{
    if (!root_node)
    {
        root_node = new node<TKey, TValue>(key, value);
        status = INSERT_ROOT;
    }
    else if (key_comparator(key, root_node->key) == -1)
    {
        root_node->left = insert_node_base(root_node->left, key, value, status);
        status = INSERT_LEFT;
    }
    else if (key_comparator(key, root_node->key) == 1)
    {
        root_node->right = insert_node_base(root_node->right, key, value, status);
        status = INSERT_RIGHT;
    }
    else if (key_comparator(key, root_node->key) == 0)
    {
        throw typename tree<TKey, TValue>::item_exists_exception();
    }
    return root_node;
}

template <typename TKey, typename TValue>
node<TKey, TValue> *binary_tree<TKey, TValue>::find_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status)
{
    status = FIND_ROOT;
    while(root_node)
    {
        switch (key_comparator(key, root_node->key)) {
        case -1:
            root_node = root_node->left;
            status = FIND_LEFT;
            break;
        case 1:
            root_node = root_node->right;
            status = FIND_RIGHT;
            break;
        case 0:
            return root_node;
            break;
        }
    }
    throw typename tree<TKey, TValue>::invalid_key_exception();
}

template <typename TKey, typename TValue>
node<TKey, TValue> *binary_tree<TKey, TValue>::remove_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status)
{
    node<TKey, TValue> *parent_node = nullptr;
    node<TKey, TValue>* temp_node;
    if (!root_node)
    {
        throw typename tree<TKey, TValue>::invalid_key_exception();
    }
    else if (key_comparator(key, root_node->key) == -1)
    {
        parent_node = root_node;
        root_node->left = remove_node_base(root_node->left, key, status);
        status = REMOVE_LEFT;
    }
    else if (key_comparator(key, root_node->key) == 1)
    {
        parent_node = root_node;
        root_node->right = remove_node_base(root_node->right, key, status);
        status = REMOVE_RIGHT;
    }
    else if(root_node->left && root_node->right)
    {
        temp_node = tree<TKey, TValue>::find_min_node(root_node->right);
        root_node->key = temp_node->key;
        root_node->value = temp_node->value;
        root_node->right = remove_node_base(root_node->right, root_node->key, status);
    }
    else
    {
        temp_node = root_node;
        if(!root_node->left)
        {
            root_node = root_node->right;
        }
        else if(!root_node->right)
        {
            root_node = root_node->left;
        }
        delete temp_node;
    }
    if (!parent_node)
    {
        status = REMOVE_ROOT;
    }
    return root_node;
}

#endif // BINARYTREE_H
