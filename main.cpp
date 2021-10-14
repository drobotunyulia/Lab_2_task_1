#include <iostream>
#include "binarytree.h"

using namespace std;

template<typename TKey, typename TValue>
void print(TKey key, TValue value, int depth)
{
    cout << "Key = " << key << "  :  Value = " << value << "  :  Depth = " << depth << endl;
}

void example_1()
{
    cout << "Example 1:" << endl << "TKey - int, TValue - string" << endl;
    comparator<int> comp;
    binary_tree<int, string> *tree = new binary_tree<int, string>(comp);
    try
    {
        cout << "Insert 8 : \"008\"" << endl;
        tree->insert_node(8, "008");
        cout << "Insert 7 : \"007\"" << endl;
        tree->insert_node(7, "007");
        cout << "Insert 6 : \"006\"" << endl;
        tree->insert_node(6, "006");
        cout << "Insert 5 : \"005\"" << endl;
        tree->insert_node(5, "005");
        cout << "Insert 4 : \"004\"" << endl;
        tree->insert_node(4, "004");
        cout << "Insert 1 : \"001\"" << endl;
        tree->insert_node(1, "001");
        cout << "Insert 4 : \"004\"" << endl;
        tree->insert_node(4, "004");
    }
    catch (binary_tree<int, string>::tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    cout << "Result tree:" << endl;
    tree->prefix_traversal(print<int, string>);
    cout << endl;
    try
    {
        cout << "Find 4 item: " << tree->find_value(4) << endl;
        cout << "Find 10 item: " << tree->find_value(10) << endl;
    }
    catch (binary_tree<int, string>::tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        cout << "Deleting 8\n";
        tree->remove_node(8);

        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 7\n";
        tree->remove_node(7);

        tree->prefix_traversal(print<int, string>);
        cout << endl;
        cout << "Deleting 12\n";
        tree->remove_node(12);
    }
    catch (binary_tree<int, string>::tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
}

void example_2()
{
    cout << "Example 2:" << endl << "TKey - string, TValue - string" << endl;
    comparator<string> comp;
    binary_tree<string, string> *tree = new binary_tree<string, string>(comp);
    try
    {
        cout << "Insert \"008\" : \"008\"" << endl;
        tree->insert_node("008", "008");
        cout << "Insert \"007\" : \"007\"" << endl;
        tree->insert_node("007", "007");
        cout << "Insert \"006\" : \"006\"" << endl;
        tree->insert_node("006", "006");
        cout << "Insert \"005\" : \"005\"" << endl;
        tree->insert_node("005", "005");
        cout << "Insert \"004\" : \"004\"" << endl;
        tree->insert_node("004", "004");
        cout << "Insert \"001\" : \"001\"" << endl;
        tree->insert_node("001", "001");
        cout << "Insert \"004\" : \"004\"" << endl;
        tree->insert_node("004", "004");
    }
    catch (binary_tree<string, string>::tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    cout << "Result tree:" << endl;
    tree->prefix_traversal(print<string, string>);
    cout << endl;
    try
    {
        cout << "Find 004 item: " << tree->find_value("004") << endl;
        cout << "Find 010 item: " << tree->find_value("010") << endl;
    }
    catch (binary_tree<string, string>::tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
    try
    {
        cout << "Deleting 008\n";
        tree->remove_node("008");

        tree->prefix_traversal(print<string, string>);
        cout << endl;
        cout << "Deleting 007\n";
        tree->remove_node("007");

        tree->prefix_traversal(print<string, string>);
        cout << endl;
        cout << "Deleting 012\n";
        tree->remove_node("012");
    }
    catch (binary_tree<string, string>::tree_exception &exception)
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
