#include "tree.c"

bool insert_test()
{
    splay_tree * tree = new_tree();
    int a[5] = {1, 2, 3, 4, 5};
    int index = 0;
    while (index < 5)
    {
        insert(tree, &a[index]);
        ++index;
    }
    index = 0;
    while (index < 5)
    {
        if (!search(tree, &a[index]))
        {
            return false;
        }
        ++index;
    }
    int b[] = {10, 11, 12, 13, 14};
    index = 0;
    while (index < 5)
    {
        if (search(tree, &b[index]))
        {
            return false;
        }
        ++index;
    }
    return true;
}

bool empty_test()
{
    splay_tree * tree = new_tree();
    return is_empty(tree);
}

bool delete_test()
{
    splay_tree * tree = new_tree();
    int elements_to_insert[10];
    for (int index=0; index < 10; ++index)
    {
        elements_to_insert[index] = index + 1;  // 1 - 10
        insert(tree, &elements_to_insert[index]);
    }
    for (int index=0; index < 10 / 2; ++index)
    {
        remove_node(tree, &elements_to_insert[index]);
    }
    for (int index=0; index < 10; ++index)
    {
        if (index < 5 && search(tree, &elements_to_insert[index])) return false;
        if (index > 5 && !search(tree, &elements_to_insert[index])) return false;
    }
    return true;
}

bool max_min_test()
{
    splay_tree * tree = new_tree();
    int elements[] = {1, 2, 10, 4, 5};
    for (int index = 0; index < 5; ++index)
    {
        insert(tree, &elements[index]);
    }
    bool max_test = 10 == maximum(tree->root)->data;
    bool min_test = 1 == minimum(tree->root)->data;
    return max_test == true && min_test == true;
}

bool successor_predecessor_test()
{
    splay_tree * tree = new_tree();
    int elements[] = {1, 2, 3, 4, 5};
    for (int index = 0; index < 5; ++index)
    {
        insert(tree, &elements[index]);
    }
    if (5 != successor(tree, &elements[3]) || 4 != predecessor(tree, &elements[4]))
    {
        return false;
    }
    return true;
}
