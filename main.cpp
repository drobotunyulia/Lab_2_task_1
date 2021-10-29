#include <iostream>
#include "binarytree.h"
using namespace std;

//определяем функцию обратного вызова
template<typename TKey, typename TValue>
void print(TKey key, TValue value, int depth)
{
    cout << "Key = " << key << "  :  Value = " << value << "  :  Depth = " << depth << endl;
}

void example_1()
{
    //пример бинарного дерева с ключем типа "int" и данными типа "string"
    cout << "Example 1:" << endl << "TKey - int, TValue - string" << endl;
    //создаем экземпляр класса шаблонного метода поиска
    find_template_method<int, string> finder;
    //создаем экземпляр класса шаблонного метода вставки
    insert_template_method<int, string> inserter;
    //создаем экземпляр класса шаблонного метода удаления
    remove_template_method<int, string> remover;
    //создаем экземпляр компаратора
    comparator<int> comparator;
    //создаем экземпляр бинарного дерева
    //в конструктор передаем созданные ранее экземпляры классов шаблонных методов и компаратора
    binary_tree<int, string> *tree = new binary_tree<int, string>(finder, inserter, remover, comparator);
    try
    {
        cout << "Insert 8 : \"008\"" << endl;
        tree->insert(8, "008");
        cout << "Insert 7 : \"007\"" << endl;
        tree->insert(7, "007");
        cout << "Insert 6 : \"006\"" << endl;
        tree->insert(6, "006");
        cout << "Insert 5 : \"005\"" << endl;
        tree->insert(5, "005");
        cout << "Insert 4 : \"004\"" << endl;
        tree->insert(4, "004");
        cout << "Insert 1 : \"001\"" << endl;
        tree->insert(1, "001");
        cout << "Insert 4 : \"004\"" << endl;
        tree->insert(4, "004");
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    cout << "Result tree:" << endl;
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    try
    {
        cout << "Find 4 item: " << tree->find(4) << endl;
        cout << "Find 10 item: " << tree->find(10) << endl;
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        cout << "Deleting 8\n";
        tree->remove(8);

        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 7\n";
        tree->remove(7);

        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 12\n";
        tree->remove(12);
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
}

void example_2()
{
    //пример бинарного дерева с ключем типа "string" и данными типа "string"
    cout << "Example 2:" << endl << "TKey - string, TValue - string" << endl;
    find_template_method<string, string> finder;
    insert_template_method<string, string> inserter;
    remove_template_method<string, string> remover;
    comparator<string> comparator;
    binary_tree<string, string> *tree = new binary_tree<string, string>(finder, inserter, remover, comparator);
    try
    {
        cout << "Insert \"008\" : \"008\"" << endl;
        tree->insert("008", "008");
        cout << "Insert \"007\" : \"007\"" << endl;
        tree->insert("007", "007");
        cout << "Insert \"006\" : \"006\"" << endl;
        tree->insert("006", "006");
        cout << "Insert \"005\" : \"005\"" << endl;
        tree->insert("005", "005");
        cout << "Insert \"004\" : \"004\"" << endl;
        tree->insert("004", "004");
        cout << "Insert \"001\" : \"001\"" << endl;
        tree->insert("001", "001");
        cout << "Insert \"004\" : \"004\"" << endl;
        tree->insert("004", "004");
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    cout << "Result tree:" << endl;
    tree->prefix_traversal(print<string, string>);
    cout << endl;
    try
    {
        cout << "Find 004 item: " << tree->find("004") << endl;
        cout << "Find 010 item: " << tree->find("010") << endl;
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        cout << "Deleting 008\n";
        tree->remove("008");

        tree->prefix_traversal(print<string, string>);
        cout << endl;
        cout << "Deleting 007\n";
        tree->remove("007");

        tree->prefix_traversal(print<string, string>);
        cout << endl;
        cout << "Deleting 012\n";
        tree->remove("012");
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
}

int main()
{
    example_1();
    getchar();
    example_2();
    return 0;
}

/*int main(int argc, char *argv[])
{
    find_template_method<int, string> binary_find;
    insert_template_method<int, string> binary_insert;
    remove_template_method<int, string> binary_remove;
    comparator<int> binary_comparator;
    binary_tree<int, string> *tree = new binary_tree<int, string>(binary_find, binary_insert, binary_remove, binary_comparator);
    binary_tree<int, string> *tree_copy = new binary_tree<int, string>(*tree);
    tree->insert(3, "45");
    tree->insert(4, "45");
    tree->insert(5, "45");
    tree->insert(6, "45");
    tree->insert(7, "45");
    tree->insert(8, "45");
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    tree->remove(4);
    tree->remove(3);
    tree->remove(5);
    tree->remove(6);
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    tree->remove(7);
    tree->remove(8);
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    tree_copy->insert(10, "45");
    tree_copy->insert(11, "45");
    tree_copy->insert(12, "45");
    tree_copy->remove(12);
    tree_copy->prefix_traversal(print<int, string>);
    return 0;
}*/