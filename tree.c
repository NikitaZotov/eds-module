#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct SplayTree
{
    struct SplayNode
    {
        struct SplayNode *leftChild;
        struct SplayNode *rightChild;
        struct SplayNode *parent;
        int data;
    } * root;
};

struct SplayNode *newNode(int *data)
{
    struct SplayNode *node = malloc(sizeof(*node));
    node->data = *data;
    node->leftChild = node->rightChild = NULL;
    return node;
}

void deleteNode(struct SplayNode *node)
{
    free(node->leftChild);
    free(node->rightChild);
}

struct SplayNode *_Minimum(struct SplayNode *localRoot)
{
    struct SplayNode *minimum = localRoot;

    while (minimum->leftChild != NULL)
        minimum = minimum->leftChild;

    return minimum;
}

struct SplayNode *_Maximum(struct SplayNode *localRoot)
{
    struct SplayNode *maximum = localRoot;

    while (maximum->rightChild != NULL)
        maximum = maximum->rightChild;

    return maximum;
}

struct SplayNode *_Predecessor(struct SplayTree *tree, struct SplayNode *localRoot)
{
    struct SplayNode *predecessor = localRoot;
    if (predecessor->leftChild != NULL)
    {
        predecessor = _Maximum(predecessor->leftChild);
    }
    else
    {
        while (predecessor != tree->root || predecessor != predecessor->parent->rightChild)
        {
            predecessor = predecessor->parent;
        }
    }
    return predecessor;
}

struct SplayNode *_Successor(struct SplayTree *tree, struct SplayNode *localRoot)
{
    struct SplayNode *successor = localRoot;
    if (successor->rightChild != NULL)
    {
        successor = _Minimum(successor->rightChild);
    }
    else
    {
        while (successor != tree->root || successor != successor->parent->leftChild)
        {
            successor = successor->parent;
        }
    }
    return successor;
}

void _Transplant(struct SplayTree *tree, struct SplayNode *localParent, struct SplayNode *localChild)
{
    if (localParent->parent == NULL)
        tree->root = localChild;
    else if (localParent == localParent->parent->leftChild)
        localParent->parent->leftChild = localChild;
    else if (localParent == localParent->parent->rightChild)
        localParent->parent->rightChild = localChild;

    if (localChild != NULL)
        localChild->parent = localParent->parent;
}

void _LeftRotate(struct SplayTree *tree, struct SplayNode *localRoot)
{
    struct SplayNode *rightChild = localRoot->rightChild;
    localRoot->rightChild = rightChild->leftChild;
    if (rightChild->leftChild != NULL)
        rightChild->leftChild->parent = localRoot;

    _Transplant(tree, localRoot, rightChild);

    rightChild->leftChild = localRoot;
    rightChild->leftChild->parent = rightChild;
}

void _RightRotate(struct SplayTree *tree, struct SplayNode *localRoot)
{
    struct SplayNode *leftChild = localRoot->leftChild;

    localRoot->leftChild = leftChild->rightChild;
    if (leftChild->rightChild != NULL)
        leftChild->rightChild->parent = localRoot;

    _Transplant(tree, localRoot, leftChild);

    leftChild->rightChild = localRoot;
    leftChild->rightChild->parent = leftChild;
}

void _Splay(struct SplayTree *tree, struct SplayNode *pivotElement)
{
    while (pivotElement != tree->root)
    {
        if (pivotElement->parent == tree->root)
        {
            if (pivotElement == pivotElement->parent->leftChild)
            {
                _RightRotate(tree, pivotElement->parent);
            }

            else if (pivotElement == pivotElement->parent->rightChild)
            {
                _LeftRotate(tree, pivotElement->parent);
            }
        }
        else
        {
            // Zig-Zig step.
            if (pivotElement == pivotElement->parent->leftChild &&
                pivotElement->parent->parent != NULL &&
                pivotElement->parent == pivotElement->parent->parent->leftChild)
            {

                _RightRotate(tree, pivotElement->parent->parent);
                _RightRotate(tree, pivotElement->parent);
            }
            
            else if (pivotElement->parent->parent != NULL &&
                     pivotElement->parent->parent && pivotElement == pivotElement->parent->rightChild &&
                     pivotElement->parent == pivotElement->parent->parent->rightChild)
            {
                _LeftRotate(tree, pivotElement->parent->parent);
                _LeftRotate(tree, pivotElement->parent);
            }
            // Zig-Zag step.
            else if (pivotElement == pivotElement->parent->rightChild &&
                     pivotElement->parent->parent != NULL &&
                     pivotElement->parent == pivotElement->parent->parent->leftChild)
            {

                _LeftRotate(tree, pivotElement->parent);
                _RightRotate(tree, pivotElement->parent);
            }
            else if (pivotElement == pivotElement->parent->leftChild &&
                     pivotElement->parent->parent != NULL &&
                     pivotElement->parent == pivotElement->parent->parent->rightChild)
            {

                _RightRotate(tree, pivotElement->parent);
                _LeftRotate(tree, pivotElement->parent);
            }
        }
    }
}

struct SplayNode *_Search(struct SplayTree *tree, int *key)
{
    struct SplayNode *searchedElement = tree->root;

    while (searchedElement != NULL)
    {
        if (searchedElement->data < *key)
            searchedElement = searchedElement->rightChild;
        else if (*key < searchedElement->data)
            searchedElement = searchedElement->leftChild;
        else if (searchedElement->data == *key)
        {
            _Splay(tree, searchedElement);
            return searchedElement;
        }
    }
    return NULL;
}

struct SplayTree *newTree()
{
    struct SplayTree *tree = malloc(sizeof(*tree));
    tree->root = malloc(sizeof(*tree->root));
    tree->root = NULL;
    return tree;
}

void deleteTree(struct SplayTree *tree)
{
    free(tree);
}

void Insert(struct SplayTree *tree, int *key)
{
    struct SplayNode *preInsertPlace = malloc(sizeof(*preInsertPlace));
    preInsertPlace = NULL;
    struct SplayNode *insertPlace = tree->root;

    while (insertPlace != NULL)
    {
        preInsertPlace = insertPlace;

        if (insertPlace->data < *key)
            insertPlace = insertPlace->rightChild;
        else if (*key <= insertPlace->data)
            insertPlace = insertPlace->leftChild;
    }
    struct SplayNode *insertElement = newNode(key);
    insertElement->parent = preInsertPlace;
    if (preInsertPlace == NULL)
        tree->root = insertElement;
    else if (preInsertPlace->data < insertElement->data)
        preInsertPlace->rightChild = insertElement;
    else if (insertElement->data < preInsertPlace->data)
        preInsertPlace->leftChild = insertElement;
    _Splay(tree, insertElement);
}

void Remove(struct SplayTree *tree, int *key)
{
    struct SplayNode *removeElement = _Search(tree, key);

    if (removeElement != NULL)
    {
        if (removeElement->rightChild == NULL)
            _Transplant(tree, removeElement, removeElement->leftChild);
        else if (removeElement->leftChild == NULL)
            _Transplant(tree, removeElement, removeElement->rightChild);
        else
        {
            struct SplayNode *newLocalRoot = _Minimum(removeElement->rightChild);

            if (newLocalRoot->parent != removeElement)
            {

                _Transplant(tree, newLocalRoot, newLocalRoot->rightChild);

                newLocalRoot->rightChild = removeElement->rightChild;
                newLocalRoot->rightChild->parent = newLocalRoot;
            }

            _Transplant(tree, removeElement, newLocalRoot);

            newLocalRoot->leftChild = removeElement->leftChild;
            newLocalRoot->leftChild->parent = newLocalRoot;

            _Splay(tree, newLocalRoot);
        }

        free(removeElement);
    }
}

bool Search(struct SplayTree *tree, int *key)
{
    return _Search(tree, key) != NULL;
}

bool isEmpty(struct SplayTree *tree)
{
    return tree->root == NULL;
}

int Successor(struct SplayTree *tree, int *key)
{
    if (_Successor(tree, _Search(tree, key)) != NULL)
    {
        return _Successor(tree, _Search(tree, key))->data;
    }
    else
    {
        return -1;
    }
}

int Predecessor(struct SplayTree *tree, int *key)
{
    if (_Predecessor(tree, _Search(tree, key)) != NULL)
    {
        return _Predecessor(tree, _Search(tree, key))->data;
    }
    else
    {
        return -1;
    }
}

int main(int argc, char const *argv[])
{
    struct SplayTree *tree = newTree();
    int a = 8;
    int b = 10;
    Insert(tree, &a);
    Insert(tree, &b);
    printf("%d", Search(tree, &b));
    return 0;
}
