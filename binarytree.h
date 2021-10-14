#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <exception>
#include <iostream>
#include <vector>
#include <functional>

#include "comparator.h"

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
struct node
{
    TKey key;
    TValue value;
    int height;
    int color;
    node *left = nullptr;
    node *right = nullptr;
    node();
    node(TKey key, TValue value);
    node &operator = (const node &node);
};

template <typename TKey, typename TValue>
class binary_tree
{
public:
    class tree_exception : public std::exception
    {
    protected:
        std::string exception_message;
    public:
        tree_exception(std::string exception_message);
        const char *what() const noexcept;
    };
    typedef std::function<void(TKey key, TValue value, int depth)> callback_function;
    binary_tree();
    binary_tree(comparator<TKey> &key_comparator);
    binary_tree(TKey root_key, TValue root_value, comparator<TKey> &key_comparator);
    binary_tree(binary_tree<TKey, TValue> &tree);//конструктор копирования
    ~binary_tree();
    binary_tree& operator = (const binary_tree &tree);
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
    virtual node<TKey, TValue> *insert_node_base(node<TKey, TValue> *root_node, TKey key, TValue value, status_t &status);
    virtual node<TKey, TValue> *find_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status);
    virtual node<TKey, TValue> *remove_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status);
    void prefix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth);
    void postfix_traversal_base(node<TKey, TValue> *root_node, callback_function function, int depth);
    void infix_traversal_base(node<TKey, TValue> *roott_node, callback_function function, int depth);
    node<TKey, TValue> *get_parent(node<TKey, TValue> *root_node, TKey key);//на будущее для получения указателя на родительский элемент
    node<TKey, TValue> *find_min_node(node<TKey, TValue>* root_node);//поиск узла с минимальным ключом
    virtual void insert_node_hook_prev();
    virtual void insert_node_hook_post(int status);
    virtual void remove_node_hook_prev();
    virtual void remove_node_hook_post(int status);
    virtual void find_node_hook_prev();
    virtual void find_node_hook_post(int status);
};

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree()
{

}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(comparator<TKey> &key_comparator)
{
    this->key_comparator = key_comparator;
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(TKey root_key, TValue root_value, comparator<TKey> &key_comparator)
{
    this->key_comparator = key_comparator;
    this->root_node = new node<TKey, TValue>(root_key, root_value);
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::binary_tree(binary_tree<TKey, TValue> &tree)
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
binary_tree<TKey, TValue>::~binary_tree()
{
    std::vector<node<TKey, TValue>> nodes;
    prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->remove_node(nodes[i].key);
    }
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>& binary_tree<TKey, TValue>::operator = (const binary_tree &tree)
{
    key_comparator = tree.key_comparator;
    std::vector<node<TKey, TValue>> nodes;
    tree.prefix_traversal([&nodes](TKey key, TValue value, int depth) { nodes.push_back({ key, value }); });
    for (size_t i = 0; i < nodes.size(); i++)
    {
        this->insert_node(nodes[i]);
    }
    return *this;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert_node(TKey key, TValue value)
{
    status_t status;
    insert_node_hook_prev();
    root_node = insert_node_base(root_node, key, value, status);
    insert_node_hook_post(status);
}

template <typename TKey, typename TValue>
node<TKey, TValue> *binary_tree<TKey, TValue>::find_node(TKey key)
{
    status_t status;
    node<TKey, TValue> *result_node = new node<TKey, TValue>();
    find_node_hook_prev();
    result_node = find_node_base(root_node, key, status);
    find_node_hook_post(status);
    return result_node;
}

template <typename TKey, typename TValue>
TValue binary_tree<TKey, TValue>::find_value(TKey key)
{
    return find_node(key)->value;
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove_node(TKey key)
{
    status_t status;
    remove_node_hook_prev();
    root_node = remove_node_base(root_node, key, status);
    remove_node_hook_post(status);
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
    else if (key_comparator(root_node->key, key) == 0)
    {
        throw tree_exception("Item already exists...");
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
    throw tree_exception("Invalid key...");
}

template <typename TKey, typename TValue>
node<TKey, TValue> *binary_tree<TKey, TValue>::remove_node_base(node<TKey, TValue> *root_node, TKey key, status_t &status)
{
    node<TKey, TValue>* temp_node;
    if (!root_node)
    {
        throw tree_exception("Invalid key...");
    }
    else if (key_comparator(key, root_node->key) == -1)
    {
        root_node->left = remove_node_base(root_node->left, key, status);
        status = REMOVE_LEFT;
    }
    else if (key_comparator(key, root_node->key) == 1)
    {
        root_node->right = remove_node_base(root_node->right, key, status);
        status = REMOVE_RIGHT;
    }
    else if(root_node->left && root_node->right)
    {
        temp_node = find_min_node(root_node->right);
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
    if (!get_parent(root_node, root_node->key))
    {
        status = REMOVE_ROOT;
    }
    return root_node;
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

template <typename TKey, typename TValue>
node<TKey, TValue> *binary_tree<TKey, TValue>::get_parent(node<TKey, TValue> *root_node, TKey key)
{
    node<TKey, TValue> *current_node = root_node;
    node<TKey, TValue> *parent_node = nullptr;
    while (current_node && current_node->key != key)
    {
        parent_node = current_node;
        if (key_comparator(key, current_node->key) == -1)
        {
            current_node = current_node->left;
        }
        else if (key_comparator(key, current_node->key) == 1)
        {
             current_node = current_node->right;
        }
    }
    if (!current_node)
    {
        throw tree_exception("Invalid key...");
    }
    return parent_node;
}

template <typename TKey, typename TValue>
node<TKey, TValue>* binary_tree<TKey, TValue>::find_min_node(node<TKey, TValue>* root_node)
{
    if(!root_node)
        return root_node;
    else if(!root_node->left)
        return root_node;
    else
        return find_min_node(root_node->left);
}

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert_node_hook_prev()
{
};

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::insert_node_hook_post(int status)
{
};

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove_node_hook_prev()
{
};

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::remove_node_hook_post(int status)
{
};

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::find_node_hook_prev()
{
};

template <typename TKey, typename TValue>
void binary_tree<TKey, TValue>::find_node_hook_post(int status)
{
};

template <typename TKey, typename TValue>
node<TKey, TValue>::node()
{

}

template <typename TKey, typename TValue>
node<TKey, TValue>::node(TKey key, TValue value)
{
    this->key = key;
    this->value = value;
}

template <typename TKey, typename TValue>
node<TKey, TValue>& node<TKey, TValue>::operator = (const node &node)
{
    this->key = node.key;
    this->value = node.value;
    this->height = node.height;
    this->color = node.color;
    this->left = node.left;
    this->right = node.right;
    return *this;
}

template <typename TKey, typename TValue>
binary_tree<TKey, TValue>::tree_exception::tree_exception(std::string exception_message)
{
    this->exception_message = exception_message;
}

template <typename TKey, typename TValue>
const char* binary_tree<TKey, TValue>::tree_exception::what() const noexcept
{
    return exception_message.c_str();
}
#endif // BINARYTREE_H
