#ifndef COMPARATOR_H
#define COMPARATOR_H

template <typename TKey>
class comparator
{
public:
    int operator () (const TKey &key_1, const TKey &key_2) const;
};

template <typename TKey>
int comparator<TKey>::operator () (const TKey &key_1, const TKey &key_2) const
{
    int result = 0;
    if (key_1 == key_2)
    {
        result = 0;
    }
    else if (key_1 > key_2)
    {
        result = 1;
    }
    else if (key_1 < key_2)
    {
        result = -1;
    }
    return result;
}

#endif // COMPARATOR_H
