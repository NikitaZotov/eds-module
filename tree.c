#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int error_value = -1;

typedef struct splay_node
{
    struct splay_node * left;
    struct splay_node * right;
    struct splay_node * parent;
    int data;
} splay_node;

typedef struct splay_tree
{
    splay_node * root;
} splay_tree;

splay_node * new_node(int * data)
{
    splay_node * node = malloc(sizeof( * node));
    node->data = * data;
    node->left = node->right = NULL;
    return node;
}

void delete_node(splay_node * node)
{
    free(node->left);
    free(node->right);
}

splay_node * minimum(splay_node * localRoot)
{
    splay_node * minimum = localRoot;

    while (minimum->left != NULL)
        minimum = minimum->left;

    return minimum;
}

splay_node * maximum(splay_node * local_root)
{
    splay_node * maximum = local_root;

    while (maximum->right != NULL)
        maximum = maximum->right;

    return maximum;
}

splay_node * _predecessor(splay_tree * tree, splay_node * local_root)
{
    splay_node *_predecessor = local_root;
    if (_predecessor->left != NULL)
    {
        _predecessor = maximum(_predecessor->left);
    }
    else
    {
        while (_predecessor != tree->root || _predecessor != _predecessor->parent->right)
        {
            _predecessor = _predecessor->parent;
        }
    }
    return _predecessor;
}

splay_node * _successor(splay_tree * tree, splay_node * local_root)
{
    splay_node * _successor = local_root;
    if (_successor->right != NULL)
    {
        _successor = minimum(_successor->right);
    }
    else
    {
        while (_successor != tree->root || _successor != _successor->parent->left)
        {
            _successor = _successor->parent;
        }
    }
    return _successor;
}

void transplant(splay_tree * tree, splay_node * local_parent, splay_node * local_child)
{
    if (local_parent->parent == NULL)
        tree->root = local_child;
    else if (local_parent == local_parent->parent->left)
        local_parent->parent->left = local_child;
    else if (local_parent == local_parent->parent->right)
        local_parent->parent->right = local_child;

    if (local_child != NULL)
        local_child->parent = local_parent->parent;
}

void left_rotate(splay_tree * tree, splay_node * local_root)
{
    splay_node * right = local_root->right;
    local_root->right = right->left;
    if (right->left != NULL)
        right->left->parent = local_root;

    transplant(tree, local_root, right);

    right->left = local_root;
    right->left->parent = right;
}

void right_rotate(splay_tree * tree, splay_node * local_root)
{
    splay_node * left = local_root->left;

    local_root->left = left->right;
    if (left->right != NULL)
        left->right->parent = local_root;

    transplant(tree, local_root, left);

    left->right = local_root;
    left->right->parent = left;
}

void splay(splay_tree * tree, splay_node * pivot_element)
{
    while (pivot_element != tree->root)
    {
        if (pivot_element->parent == tree->root)
        {
            if (pivot_element == pivot_element->parent->left)
            {
                right_rotate(tree, pivot_element->parent);
            }

            else if (pivot_element == pivot_element->parent->right)
            {
                left_rotate(tree, pivot_element->parent);
            }
        }
        else
        {
            // Zig-Zig step.
            if (pivot_element == pivot_element->parent->left &&
                pivot_element->parent->parent != NULL &&
                pivot_element->parent == pivot_element->parent->parent->left)
            {

                right_rotate(tree, pivot_element->parent->parent);
                right_rotate(tree, pivot_element->parent);
            }

            else if (pivot_element->parent->parent != NULL &&
                     pivot_element->parent->parent && pivot_element == pivot_element->parent->right &&
                     pivot_element->parent == pivot_element->parent->parent->right)
            {
                left_rotate(tree, pivot_element->parent->parent);
                left_rotate(tree, pivot_element->parent);
            }
            // Zig-Zag step.
            else if (pivot_element == pivot_element->parent->right &&
                     pivot_element->parent->parent != NULL &&
                     pivot_element->parent == pivot_element->parent->parent->left)
            {

                left_rotate(tree, pivot_element->parent);
                right_rotate(tree, pivot_element->parent);
            }
            else if (pivot_element == pivot_element->parent->left &&
                     pivot_element->parent->parent != NULL &&
                     pivot_element->parent == pivot_element->parent->parent->right)
            {

                right_rotate(tree, pivot_element->parent);
                left_rotate(tree, pivot_element->parent);
            }
        }
    }
}

splay_node * _search(splay_tree * tree, int * key)
{
    splay_node * _searchedElement = tree->root;

    while (_searchedElement != NULL)
    {
        if (_searchedElement->data < * key)
            _searchedElement = _searchedElement->right;
        else if ( * key < _searchedElement->data)
            _searchedElement = _searchedElement->left;
        else if (_searchedElement->data == * key)
        {
            splay(tree, _searchedElement);
            return _searchedElement;
        }
    }
    return NULL;
}

splay_tree * new_tree()
{
    splay_tree * tree = malloc(sizeof( * tree));
    tree->root = malloc(sizeof( * tree->root));
    tree->root = NULL;
    return tree;
}

void delete_tree(splay_tree * tree)
{
    free(tree);
}

void insert(splay_tree * tree, int * key)
{
    splay_node * pre_insert_place = malloc(sizeof( * pre_insert_place));
    pre_insert_place = NULL;
    splay_node * insert_place = tree->root;

    while (insert_place != NULL)
    {
        pre_insert_place = insert_place;

        if (insert_place->data < * key)
            insert_place = insert_place->right;
        else if ( * key <= insert_place->data)
            insert_place = insert_place->left;
    }
    splay_node * insert_element = new_node(key);
    insert_element->parent = pre_insert_place;
    if (pre_insert_place == NULL)
        tree->root = insert_element;
    else if (pre_insert_place->data < insert_element->data)
        pre_insert_place->right = insert_element;
    else if (insert_element->data < pre_insert_place->data)
        pre_insert_place->left = insert_element;
    splay(tree, insert_element);
}

void remove_node(splay_tree * tree, int * key)
{
    splay_node * removeElement = _search(tree, key);

    if (removeElement != NULL)
    {
        if (removeElement->right == NULL)
            transplant(tree, removeElement, removeElement->left);
        else if (removeElement->left == NULL)
            transplant(tree, removeElement, removeElement->right);
        else
        {
            splay_node *newLocalRoot = minimum(removeElement->right);

            if (newLocalRoot->parent != removeElement)
            {

                transplant(tree, newLocalRoot, newLocalRoot->right);

                newLocalRoot->right = removeElement->right;
                newLocalRoot->right->parent = newLocalRoot;
            }

            transplant(tree, removeElement, newLocalRoot);

            newLocalRoot->left = removeElement->left;
            newLocalRoot->left->parent = newLocalRoot;

            splay(tree, newLocalRoot);
        }

        free(removeElement);
    }
}

bool is_empty(splay_tree * tree)
{
    return tree->root == NULL;
}

int successor(splay_tree * tree, int * key)
{
    if (_successor(tree, _search(tree, key)) != NULL)
    {
        return _successor(tree, _search(tree, key))->data;
    }
    else
    {
        return error_value;
    }
}

int predecessor(splay_tree * tree, int * key)
{
    if (_predecessor(tree, _search(tree, key)) != NULL)
    {
        return _predecessor(tree, _search(tree, key))->data;
    }
    else
    {
        return error_value;
    }
}

bool search(splay_tree * tree, int * key)
{
    if (_search(tree, key) != NULL){
        return true;
    }
    return false;
}
