#ifndef NODE_H
#define NODE_H

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

#endif // NODE_H
