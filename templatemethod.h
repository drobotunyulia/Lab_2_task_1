#ifndef TEMPLATEMETHOD_H
#define TEMPLATEMETHOD_H

#include "node.h"
#include "comparator.h"

enum status_t {
    FIND_SUCCESS,
    FIND_ERROR,
    INSERT_SUCCESS,
    INSERT_ERROR,
    REMOVE_SUCCESS,
    REMOVE_ERROR
};

template <typename TKey, typename TValue>
node<TKey, TValue> *get_left(node<TKey, TValue> *p_node)
{
    if (p_node)
    {
        return p_node->left;
    }
    return nullptr;
}

template <typename TKey, typename TValue>
node<TKey, TValue> *get_right(node<TKey, TValue> *p_node)
{
    if (p_node)
    {
        return p_node->right;
    }
    return nullptr;
}


template <typename TKey, typename TValue>
node<TKey, TValue> *find_min_node(node<TKey, TValue>* root_node)
{
    if(!root_node)
    {
        return nullptr;
    }
    else if(!root_node->left)
    {
        return root_node;
    }
    else
    {
        return find_min_node(root_node->left);
    }
}

template <typename TKey, typename TValue>
node<TKey, TValue> *get_parent(node<TKey, TValue> *root_node, node<TKey, TValue> *p_node, comparator<TKey> key_comparator)
{
    node<TKey, TValue> *parent_node = nullptr;
    if (!root_node)
    {
        return nullptr;
    }
    if (!p_node)
    {
        return nullptr;
    }
    while (root_node && root_node->key != p_node->key)
    {
        parent_node = root_node;
        if (key_comparator(p_node->key, root_node->key) == -1)
        {
            root_node = root_node->left;
        }
        else if (key_comparator(p_node->key, root_node->key) == 1)
        {
            root_node = root_node->right;
        }
    }
    return parent_node;
}


template <typename TKey, typename TValue>
class find_template_method
{
public:
    node<TKey, TValue> *invoke_find(node<TKey, TValue> *&root_node,
                       TKey key,
                       comparator<TKey> &key_comparator,
                       status_t &status);
protected:
    virtual node<TKey, TValue> *inner_find(node<TKey, TValue> *&root_node,
                                           TKey key,
                                           comparator<TKey> &key_comparator,
                                           status_t &status);
    virtual void post_find_hook(node<TKey, TValue> *&root_node,
                           node<TKey, TValue> *find_node,
                           comparator<TKey> &key_comparator,
                           status_t &status)
    {
    }
};

template <typename TKey, typename TValue>
node<TKey, TValue> *find_template_method<TKey, TValue>::invoke_find(node<TKey, TValue> *&root_node,
                                                                    TKey key,
                                                                    comparator<TKey> &key_comparator,
                                                                    status_t &status)
{
    node<TKey, TValue> *find_node = inner_find(root_node, key, key_comparator, status);
    post_find_hook(root_node, find_node, key_comparator, status);
    return find_node;
}

template <typename TKey, typename TValue>
node<TKey, TValue>* find_template_method<TKey, TValue>::inner_find(node<TKey, TValue> *&root_node,
                                                                   TKey key,
                                                                   comparator<TKey> &key_comparator,
                                                                   status_t &status)
{
    status = FIND_SUCCESS;
    node<TKey, TValue> *current_node = root_node;
    while(current_node)
    {
        switch (key_comparator(key, current_node->key)) {
        case -1:
            current_node = current_node->left;
            break;
        case 1:
            current_node = current_node->right;
            break;
        case 0:
            return current_node;
            break;
        }
    }
    status = FIND_ERROR;
    return nullptr;
}

template <typename TKey, typename TValue>
class insert_template_method
{
public:
    void invoke_insert(node<TKey, TValue> *&root_node, TKey key,
                       TValue value,
                       comparator<TKey> &key_comparator,
                       status_t &status);
protected:
    virtual node<TKey, TValue> *inner_insert(node<TKey, TValue> *&root_node,
                                             TKey key,
                                             TValue value,
                                             comparator<TKey> &key_comparator,
                                             status_t &status);
    virtual void post_insert_hook(node<TKey, TValue> *&root_node,
                                  node<TKey, TValue> *insert_node,
                                  comparator<TKey> &key_comparator,
                                  status_t &status)
    {
    }
};

template <typename TKey, typename TValue>
void insert_template_method<TKey, TValue>::invoke_insert(node<TKey, TValue> *&root_node,
                                                         TKey key,
                                                         TValue value,
                                                         comparator<TKey> &key_comparator,
                                                         status_t &status)
{
    node<TKey, TValue> *insert_node = inner_insert(root_node, key, value, key_comparator, status);
    post_insert_hook(root_node, insert_node, key_comparator, status);
}

template <typename TKey, typename TValue>
node<TKey, TValue> *insert_template_method<TKey, TValue>::inner_insert(node<TKey, TValue> *&root_node,
                                                                       TKey key,
                                                                       TValue value,
                                                                       comparator<TKey> &key_comparator,
                                                                       status_t &status)
{
    status = INSERT_SUCCESS;
    node<TKey, TValue> *insert_node = new node<TKey, TValue>(key, value);
    if (!root_node)
    {
        root_node = insert_node;
    }
    else
    {
        node<TKey, TValue> *current_node = root_node;
        node<TKey, TValue> *parent_node = nullptr;
        while (current_node)
        {
            parent_node = current_node;
            if (key_comparator(insert_node->key, current_node->key) == -1)
            {
                current_node = current_node->left;
            }
            else if (key_comparator(insert_node->key, current_node->key) == 1)
            {
                current_node = current_node->right;
            }
            else if (key_comparator(insert_node->key, current_node->key) == 0)
            {
                status = INSERT_ERROR;
                return insert_node;
            }
        }
        if (key_comparator(insert_node->key, parent_node->key) == -1)
        {
            parent_node->left = insert_node;
        }
        else if (key_comparator(insert_node->key, parent_node->key) == 1)
        {
            parent_node->right = insert_node;
        }
    }
    return insert_node;
}

template <typename TKey, typename TValue>
class remove_template_method
{
public:
    void invoke_remove(node<TKey, TValue> *&root_node,
                       TKey key,
                       comparator<TKey> &key_comparator,
                       status_t &status);
protected:
    virtual node<TKey, TValue> *inner_remove(node<TKey, TValue> *&root_node,
                                             TKey key,
                                             comparator<TKey> &key_comparator,
                                             status_t &status);
    virtual void post_remove_hook(node<TKey, TValue> *&root_node,
                             node<TKey, TValue> *replace_node,
                             comparator<TKey> &key_comparator,
                             status_t &status)
    {
    }
};

template <typename TKey, typename TValue>
void remove_template_method<TKey, TValue>::invoke_remove(node<TKey, TValue> *&root_node,
                                                         TKey key,
                                                         comparator<TKey> &key_comparator,
                                                         status_t &status)
{
    node<TKey, TValue> *replace_node = inner_remove(root_node, key, key_comparator, status);
    post_remove_hook(root_node, replace_node, key_comparator, status);
}

template <typename TKey, typename TValue>
node<TKey, TValue> *remove_template_method<TKey, TValue>::inner_remove(node<TKey, TValue> *&root_node,
                                                                       TKey key,
                                                                       comparator<TKey> &key_comparator,
                                                                       status_t &status)
{
    status = REMOVE_SUCCESS;
    node<TKey, TValue> *replace_node = nullptr;
    node<TKey, TValue> *replace_parent_node = nullptr;
    node<TKey, TValue> *current_node = root_node;
    node<TKey, TValue> *remove_node = nullptr;
    while(current_node)
    {
        if (key_comparator(key, current_node->key) == -1)
        {
            current_node = current_node->left;
        }
        else if (key_comparator(key, current_node->key) == 1)
        {
            current_node = current_node->right;
        }
        else if (key_comparator(key, current_node->key) == 0)
        {
            remove_node = current_node;
            break;
        }
    }
    if (!remove_node)
    {
        status = REMOVE_ERROR;
        return nullptr;
    }
    if (remove_node->left && remove_node->right)
    {
        replace_node = find_min_node(remove_node->right);
        replace_parent_node = get_parent(root_node, replace_node, key_comparator);
        remove_node->key = replace_node->key;
        remove_node->value = replace_node->value;
        if (!get_right(replace_node) && !get_left(replace_node))
        {
            if (replace_node == replace_parent_node->left)
            {
                replace_parent_node->left = nullptr;
            }
            else
            {
                replace_parent_node->right = nullptr;
            }
        }
        else if (get_right(replace_node))
        {
            replace_parent_node->right = replace_node->right;
        }
    }
    else if (remove_node->left)
    {
        replace_node = remove_node->left;
        if (get_parent(root_node, remove_node, key_comparator))
        {
            if (remove_node == get_left(get_parent(root_node, remove_node, key_comparator)))
            {
                get_parent(root_node, remove_node, key_comparator)->left = replace_node;
            }
            else
            {
                get_parent(root_node, remove_node, key_comparator)->right = replace_node;
            }
        }
        else
        {
            root_node = replace_node;
        }
    }
    else if (remove_node->right)
    {
        replace_node = remove_node->right;
        if (get_parent(root_node, remove_node, key_comparator))
        {
            if (remove_node == get_right(get_parent(root_node, remove_node, key_comparator)))
            {
                get_parent(root_node, remove_node, key_comparator)->right = replace_node;
            }
            else
            {
                get_parent(root_node, remove_node, key_comparator)->left = replace_node;
            }
        }
        else
        {
            root_node = replace_node;
        }
    }
    else
    {
        if (get_parent(root_node, remove_node, key_comparator))
        {
            if (remove_node == get_left(get_parent(root_node, remove_node, key_comparator)))
            {
                get_parent(root_node, remove_node, key_comparator)->left = replace_node;
            }
            else
            {
                get_parent(root_node, remove_node, key_comparator)->right = replace_node;
            }
        }
        else
        {
            root_node = replace_node;
        }
    }
    delete remove_node;
    return replace_node;
}



#endif // TEMPLATEMETHOD_H
