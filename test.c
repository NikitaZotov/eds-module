#include "tree.c"


bool equal_predicate(void * first_element, void * second_element)
{
    return *(unsigned int *)(first_element) == *(unsigned int *)(second_element);
}

bool bigger_predicate(void * first_element, void * second_element)
{
    return *(unsigned int *)(first_element) > *(unsigned int *)(second_element);
}

bool insert_test()
{
    splay_tree * tree = new_tree();
    unsigned int a[5] = {1, 2, 3, 4, 5};
    unsigned int index = 0;
    while (index < 5)
    {
        insert(tree, &a[index], bigger_predicate);
        ++index;
    }
    index = 0;
    while (index < 5)
    {
        bool was_found = search(tree, &a[index], bigger_predicate, equal_predicate);
        if (!was_found)
        {
            return false;
        }
        ++index;
    }

    unsigned int* b; 
    b = calloc(10, sizeof(unsigned int));
    for (unsigned int i = 10; i < 15; ++i)
    {
        *(b + i) = i;
    }
    index = 0;
    while (index < 5)
    {
        bool was_found = search(tree, b + index, bigger_predicate, equal_predicate)
        if (was_found)
        {
            free(b);
            return false;
        }
        ++index;
    }
    free(b);
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
    unsigned int elements_to_insert[10];
    for (unsigned int index = 0; index < 10; ++index)
    {
        elements_to_insert[index] = index + 1;  // 1 - 10
        insert(tree, &elements_to_insert[index], bigger_predicate);
    }
    for (unsigned int index = 0; index < 10 / 2; ++index)
    {
        remove_node(tree, &elements_to_insert[index], bigger_predicate, equal_predicate);
    }
    for (unsigned int index = 0; index < 10; ++index)
    {
        if (index < 5 && search(tree, &elements_to_insert[index], bigger_predicate, equal_predicate))
        {
            return false;
        }
        if (index > 5 && !search(tree, &elements_to_insert[index], bigger_predicate, equal_predicate))
        { 
            return false;
        }
    }
    return true;
}

bool max_min_test()
{
    splay_tree * tree = new_tree();
    unsigned int * elements; 
    for (unsigned int index = 0; index < 5; ++index)
    {
        *(elements + index) = index * 10;  
        insert(tree, elements + index, bigger_predicate);
    } 
    bool max_test = 40 == *(unsigned int *)maximum(tree->root)->data;
    bool min_test = 0 == *(unsigned int *)minimum(tree->root)->data;
    free(elements);
    return max_test && min_test;
}

bool successor_predecessor_test()
{
    splay_tree * tree = new_tree();
    unsigned int elements[] = {1, 2, 3, 4, 5};
    for (unsigned int index = 0; index < 5; ++index)
    {
        insert(tree, &elements[index], bigger_predicate);
    }
    if (5 != *(unsigned int *)successor(tree, &elements[3], bigger_predicate, equal_predicate) || 4 != *(unsigned int *)predecessor(tree, &elements[4], bigger_predicate, equal_predicate))
    {
        return false;
    }
    return true;
}
