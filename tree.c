#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int err = -1;
void * error_value = &err;

typedef struct splay_node
{
    struct splay_node * left;
    struct splay_node * right;
    struct splay_node * parent;
    void * data;
} splay_node;

typedef struct splay_tree
{
    splay_node * root;
} splay_tree;

/*!
 * Creates new splay node based on input data
 * @param data pointer on input data
 * @returns Returns A splay node with input data
 */
splay_node * new_node(void * data)
{
    splay_node * node = malloc(sizeof(*node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

/*!
 * Cleans memory occupied by splay node
 * @param node Node that needs to be stung
 */
void delete_node(splay_node * node)
{
    free(node->left);
    free(node->right);
}

/**
 * Finds minimal splay node from the whole tree
 * @param localRoot Splay node pointer on tree root
 * @returns Returns A minimal splay node  
 */
splay_node * minimum(splay_node * localRoot)
{
    splay_node * minimum = localRoot;

    while (minimum->left != NULL)
    {
        minimum = minimum->left;
    }

    return minimum;
}


/*!
 * Finds maximal from the whole tree
 * @param local_root Splay node pointer on tree root
 * @returns Returns A maximal splay node 
 */
splay_node * maximum(splay_node * local_root)
{
    splay_node * maximum = local_root;

    while (maximum->right != NULL)
    {
        maximum = maximum->right;
    }

    return maximum;
}

splay_node * _predecessor(splay_tree * tree, splay_node * local_root)
{
    splay_node * _predecessor = local_root;
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

/*!
 * Transplants tree based on local parent and local child
 * @param tree Splay tree used for transplanting
 * @param local_parent Splay node used as parent for transplanting
 * @param local_child Splay node used as child for transplanting
 */
void transplant(splay_tree * tree, splay_node * local_parent, splay_node * local_child)
{
    if (local_parent->parent == NULL)
    {
        tree->root = local_child;
    }
    else if (local_parent == local_parent->parent->left)
    {
        local_parent->parent->left = local_child;
    }
    else if (local_parent == local_parent->parent->right)
    {
        local_parent->parent->right = local_child;
    }

    if (local_child != NULL)
    {
        local_child->parent = local_parent->parent;
    }
}

/*!
 * Rotates tree in left order
 * @param tree Splay tree used for rotating
 * @param local_root Splay node used as root for left rotating
 */
void left_rotate(splay_tree * tree, splay_node * local_root)
{
    splay_node * right = local_root->right;
    local_root->right = right->left;
    if (right->left != NULL)
    {
        right->left->parent = local_root;
    }

    transplant(tree, local_root, right);

    right->left = local_root;
    right->left->parent = right;
}

/*!
 * Rotates tree in right order
 * @param tree Splay tree used for rotating
 * @param local_root Splay node used as root for right rotating
 */
void right_rotate(splay_tree * tree, splay_node * local_root)
{
    splay_node * left = local_root->left;

    local_root->left = left->right;
    if (left->right != NULL)
    {
        left->right->parent = local_root;
    }

    transplant(tree, local_root, left);

    left->right = local_root;
    left->right->parent = left;
}

/**
 * Splays tree based on pivot splay node 
 * @param tree Splaying tree
 * @param pivot_element Splay node used as base for rotating tree
 */
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

splay_node * _search(splay_tree * tree, void * key, bool (* bigger_predicate)(void *, void *), bool (* equal_predicate)(void *, void *))
{
    splay_node * _searchedElement = tree->root;

    while (_searchedElement != NULL)
    {
        if (bigger_predicate(key, _searchedElement->data))
        {
            _searchedElement = _searchedElement->right;
        }
        else if (equal_predicate(_searchedElement->data, key))
        {
            splay(tree, _searchedElement);
            return _searchedElement;
        }
        else
        {
            _searchedElement = _searchedElement->left;
        }
    }
    return NULL;
}

/*!
 * Creates new tree with NULL root
 * @returns Returns created tree 
 */
splay_tree * new_tree()
{
    splay_tree * tree = malloc(sizeof(*tree));
    tree->root = malloc(sizeof(*tree->root));
    tree->root = NULL;
    return tree;
}

/**
 * Cleans memory which occupied for tree 
 * @param tree Tree to remove
 */
void delete_tree(splay_tree * tree)
{
    free(tree);
}

/*!
 * Inserts splay node into tree with input key value
 * @param tree Splay tree in which the key value will be inserted
 * @param key Pointer on value which is used as data attribute value for the insertion node
 * @param bigger_predicate Predicate for comparison on equality between two values
 */
void insert(splay_tree * tree, void * key, bool (* bigger_predicate)(void *, void *))
{
    splay_node * pre_insert_place = malloc(sizeof(* pre_insert_place));
    pre_insert_place = NULL;
    splay_node * insert_place = tree->root;

    while (insert_place != NULL)
    {
        pre_insert_place = insert_place;

        if (bigger_predicate(key, insert_place->data))
        {
            insert_place = insert_place->right;
        }
        else
        {
            insert_place = insert_place->left;
        }
    }
    splay_node * insert_element = new_node(key);
    insert_element->parent = pre_insert_place;
    if (pre_insert_place == NULL)
    {
        tree->root = insert_element;
    }
    else if (bigger_predicate(insert_element->data, pre_insert_place->data))
    {
        pre_insert_place->right = insert_element;
    }
    else if (bigger_predicate(pre_insert_place->data, insert_element->data))
    {
        pre_insert_place->left = insert_element;
    }
    splay(tree, insert_element);
}

/*!
 * Removes node from splay tree node with input key data
 * @param tree Splay tree with node to remove
 * @param key Pointer on key value which has removing node as value of data attribute
 * @param bigger_predicate Predicate for comparison on bigger/lower between two values
 * @param equal_predicate Predicate for comparison on equlity between two value
 */
void remove_node(splay_tree * tree, void * key, bool (* bigger_predicate)(void *, void *), bool (* equal_predicate)(void *, void *))
{
    splay_node * removeElement = _search(tree, key, bigger_predicate, equal_predicate);

    if (removeElement != NULL)
    {
        if (removeElement->right == NULL)
        {
            transplant(tree, removeElement, removeElement->left);
        }
        else if (removeElement->left == NULL)
        {
            transplant(tree, removeElement, removeElement->right);
        }
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

/**
 * Checks if tree is empty 
 * @param tree Splay tree which is used for checking on emptiness
 * @returns Returns True or False based on emptiness of tree
 */
bool is_empty(splay_tree * tree)
{
    return tree->root == NULL;
}

/**
 * Finds successor of splay node with key value as data attribute
 * @param tree Splay tree 
 * @param key Pointer on key value used to find splay node with this key value as data attribute
 * @param bigger_predicate Predicate for comparison on bigger/lower between two values
 * @param equal_predicate Predicate for comparison on equlity between two value
 * @returns Returns successor of element with key value as data attribute
 */
void * successor(splay_tree * tree, void * key, bool (* bigger_predicate)(void *, void *), bool (* equal_predicate)(void *, void *))
{
    if (_successor(tree, _search(tree, key, bigger_predicate, equal_predicate)) != NULL)
    {
        return _successor(tree, _search(tree, key, bigger_predicate, equal_predicate))->data;
    }
    else
    {
        return (void *) &error_value;
    }
}

/*!
 * Finds predecessor of splay node with key value as data attibute
 * @param tree Splay tree 
 * @param key Pointer on key value which has splay node for which we find predcessor
 * @param bigger_predicate Predicate for comparison on bigger/lower between two values
 * @param equal_predicate Predicate for comparison on equlity between two values 
 * @returns Return data from predcessor of splay node with input key
 */
void * predecessor(splay_tree * tree, void * key, bool (* bigger_predicate)(void *, void *), bool (* equal_predicate)(void *, void *))
{
    if (_predecessor(tree, _search(tree, key, bigger_predicate, equal_predicate)) != NULL)
    {
        return _predecessor(tree, _search(tree, key, bigger_predicate, equal_predicate))->data;
    }
    else
    {
        return error_value;
    }
}

/*!
 * Looks for splay node in the tree based on key
 * @param tree Splay tree which is used for searching node with input key
 * @param key Pointer on key value which is used for splay node searching
 * @param bigger_predicate Predicate which is used for comparison for bigger/lower
 * @param equal_predicate Predicate which is used for comparison for equality
 * @returns Returns True/False based on finding node with input key
 */
bool search(splay_tree * tree, void * key, bool (* bigger_predicate)(void *, void *), bool (* equal_predicate)(void *, void *))
{
    if (_search(tree, key, bigger_predicate, equal_predicate) != NULL){
        return true;
    }
    return false;
}